using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace CAN_UDP_Monitor
{
    unsafe static class Program
    {
        //Konstante Werte und Bezeichnuingen:
        static int numKnownCmds = 9;
        static int[] knownCmds ={ 0x00, 0x04, 0x05, 0x06, 0x0b, 0x11, 0x18, 0x1B, 0x1D};
        //static int[] knownSubCmds = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        /*static string[] cmdNames = {
            "System-Befehl",
            "Lok Geschwindigkeit",
            "Lok Richtung",
            "Lok Funktion",
            "Zubehör schalten",
            "S88-Event",
            "Ping"
            "Bootloader"
        };*/

        static string[] subCmdNames = {
            "Stopp",
            "Go",
            "Halt",
            "Lok-Nothalt",
            "Lok Zyklus Stopp",
            "Lok Datenprotokoll",
            "Schaltzeit Zubehördecoder",
            "Fast Read für mfx",
            "Gleisprotokoll freischalten",
            "MFX Neuanmeldezähler setzen",
            "Überlast",
            "Status",
            "Kennung"
        };


        static void Main(string[] args)
        {
            //Port abfragen und initialisieren:

            Console.WindowWidth = 150;
            Console.WindowHeight = 40;
            Console.WriteLine();
            Console.WriteLine(" MäCAN - UDP-CAN Monitor");
            Console.WriteLine();

            int port_1 = getPort(15730, 1);
            int port_2 = getPort(15731, 2);

            Task listener1 = Task.Run(() => udpListener(port_1));
            Task listener2 = Task.Run(() => udpListener(port_2));

            listener1.Wait();
            listener2.Wait();

        }

        static void udpListener(int port)
        {
            byte[] b_datagram = new byte[13];
            string s_datagram;

            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, port);
            UdpClient client = new UdpClient(endPoint);
            Console.WriteLine("UDP-Sockel gestartet ({0}). Warte auf Pakete.", port);


            //Schleife für das eigentliche Program:
            while (true)
            {
                int cmdIndex;
                bool response;
                b_datagram = client.Receive(ref endPoint);
                s_datagram = FormatDatagram(b_datagram, port);
                Console.ForegroundColor = ConsoleColor.DarkGray;
                //Console.WriteLine();
                Console.Write(s_datagram);
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(AppDomain.CurrentDomain.BaseDirectory + "log.txt", true))
                {
                    file.WriteLine(s_datagram);
                }
                Console.ResetColor();
                if (isKnownCmd(b_datagram[1], &cmdIndex, &response) && (b_datagram[2] + b_datagram[3]) != 0)
                {
                    canDecodeBuilder(response, cmdIndex, b_datagram);
                }
                //Console.WriteLine();
                Thread.Sleep(10);
            }
        }

        //Liest eine Eingabe für einen Port ein:
        static int getPort(int defPort)
        {
            int port;
            Console.Write("Port zum empfangen angeben (Standard: {0}) >> ", defPort);
            string s_port = Console.ReadLine().ToString().Trim('\r', '\n');
            while (!validNumber(s_port))
            {
                Console.Write("Ungültiger Port! Erneut versuchen >>");
                s_port = Console.ReadLine().ToString().Trim('\r', '\n');
            }
            if (s_port == "")
            {
                port = defPort;
            }
            else
            {
                port = Convert.ToInt32(s_port);
            }
            return port;

        }

        static int getPort(int defPort, int num)
        {

            int port;
            Console.Write("{0}. Port zum empfangen angeben (Standard: {1}) >> ", num, defPort);
            string s_port = Console.ReadLine().ToString().Trim('\r', '\n');
            while (!validNumber(s_port))
            {
                Console.Write("Ungültiger Port! Erneut versuchen >>");
                s_port = Console.ReadLine().ToString().Trim('\r', '\n');
            }
            if (s_port == "")
            {
                port = defPort;
            }
            else
            {
                port = Convert.ToInt32(s_port);
            }
            return port;
        }

        //überprüft, ob ein String nur aus Zahlen besteht:
        static bool validNumber(string tmp)
        {
            string pat = "0123456789";
            foreach (char ch in tmp)
            {
                if (pat.IndexOf(ch) < 0)
                    return false;
            }
            return true;
        }

        //Formatiert das Datagram für die ausgabe auf der Konsole:
        static string FormatDatagram(byte[] b_datagram, int port)
        {
            StringBuilder s_datagram = new StringBuilder();
            s_datagram.Append("\r\n");
            s_datagram.AppendFormat("{0} >> {1} :: ID: ", port, DateTime.Now.TimeOfDay);
            for(int i = 0; i < 4; i++)
            {
                s_datagram.AppendFormat("{0:X2}", b_datagram[i]);
            }
            s_datagram.AppendFormat(" DLC: {0} Data: ", b_datagram[4]);
            for (int i = 5; i < 5+b_datagram[4]; i++)
            {
                s_datagram.AppendFormat("{0:X2} ", b_datagram[i]);
            }
            for (int i = 5+b_datagram[4]; i < 13; i++)
            {
                s_datagram.Append("---");
            }
            return s_datagram.ToString();
        }

        //Auf bekanten Befehl überprüfen:
        static bool isKnownCmd(byte can_cmd, int* index, bool* respBit)
        {
            int cmd = can_cmd >> 1;
            if (can_cmd > cmd << 1) *respBit = true; else *respBit = false;
            for(int i = 0; i < numKnownCmds; i++)
            {
                if(cmd == knownCmds[i])
                {
                    *index = i;
                    return true;
                }
            }
            return false;
        }

        static bool CS1(int hash)
        {
            if ((hash & (1 << 7)) == 0 && (hash & (1 << 8)) != 0 && (hash & (1 << 9)) != 0) return false;
            else return true;
        }

        static void canDecodeBuilder(bool cmdResp, int cmdIndex, byte[] datagram)
        {
            int hash = (datagram[2] << 8) + datagram[3];
            if (!CS1(hash))
            {
                //Console.Write("                               --> ");
                if (cmdResp)
                {
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    //Console.Write("(Antwort von 0x{0:X4}) ", hash);
                    Console.Write(" ::  Antwort --> ");
                    Console.ResetColor();
                }
                else
                {
                    Console.ForegroundColor = ConsoleColor.Cyan;
                    //Console.Write("(Befehl von 0x{0:X4}) ", hash);
                    Console.Write(" ::  Befehl  --> ");
                    Console.ResetColor();
                }

                //System-Befehle:
                if (cmdIndex == 0)
                {
                    for (int i = 0; i < 13; i++)
                    {
                        if (datagram[9] == i)
                        {
                            if (i == 0 || i == 2) { Console.Write("System-Befehl, Sub-Befehl: "); writeRed(subCmdNames[i]); }
                            else if (i == 1) { Console.Write("System-Befehl, Sub-Befehl: "); writeGreen(subCmdNames[i]); }
                            else if (i == 3 && (datagram[7] + datagram[8]) == 0) writeRed("Nothalt an alle Loks");
                            else if (i == 3) Console.Write("Lok: {0}, Nothalt", getLoco(datagram));
                            else if (i == 5) Console.Write("Lok: {0}, Protokollparameter: {1}", getLoco(datagram), datagram[10]);
                            else Console.Write("{0:X2} {1} ", datagram[9], subCmdNames[i]);
                        }

                    }
                }

                //Lok-Geschwindigkeit:
                if (cmdIndex == 1)
                {
                    float v = ((datagram[9] << 8) + datagram[10]);
                    v = v / 10;

                    Console.Write("Lok: {0}, Geschwindigkeit: {1}%", getLoco(datagram), v);
                }

                //Lok-Richtung:
                if (cmdIndex == 2)
                {
                    string dir;

                    if (datagram[4] == 4) dir = " wird abgefragt";
                    else if (datagram[9] == 0) dir = " bleibt gleich";
                    else if (datagram[9] == 1) dir = ": vorwärts";
                    else if (datagram[9] == 2) dir = ": rückwärts";
                    else if (datagram[9] == 3) dir = " wechseln";
                    else dir = "unbekannt";

                    Console.Write("Lok: {0}, Richtung{1}", getLoco(datagram), dir);
                }

                //Lok Funktion
                if (cmdIndex == 3)
                {
                    string state;
                    if (datagram[10] == 0) state = "aus"; else state = "an";
                    Console.Write("Lok: {0}, Funktion: F{1}, Status: {2}", getLoco(datagram), datagram[9], state);
                }

                //Zubehör Schalten:
                if (cmdIndex == 4)
                {
                    Console.Write("Zubehör {0}, Stellung: ", getAcc(datagram));
                    switch (datagram[9])
                    {
                        case 0x00: writeRed("rot"); break;
                        case 0xFE: writeRed("rot (Rocrail)"); break;
                        case 0x01: writeGreen("grün"); break;
                        case 0xFD: writeGreen("grün (Rocrail)"); break;
                        default: writeRed("Error..."); break;
                    }
                }

                //Ping:
                if (cmdIndex == 6 && cmdResp)
                {
                    int kenn = (datagram[11] << 8) + datagram[12];
                    int uid = (datagram[5] << 24) + (datagram[6] << 16) + (datagram[7] << 8) + datagram[8];
                    string device;
                    switch (kenn)
                    {
                        case 0x0010: device = "Gleisbox"; break;
                        case 0x0030: device = "Mobile Station 2"; break;
                        case 0x0033: device = "Mobile Station 2"; break;
                        case 0x1234: device = "MäCAN-Weichendecoder"; break;
                        case 0xFFFF: device = "CS2-GUI (Master)"; break;
                        default: device = "unbekannt"; break;
                    }
                    Console.Write("Ping-Antwort von {0} (0x{1:x4}) mit UID 0x{2:X8}, Software-Version {3}.{4}", device, kenn, uid, datagram[9], datagram[10]);

                }
                if (cmdIndex == 6 && !cmdResp) Console.Write("Ping-Anfrage");

                //Bootloader:
                if (cmdIndex == 7 && cmdResp)
                {
                    int kenn = (datagram[11] << 8) + datagram[12];
                    int uid = (datagram[5] << 24) + (datagram[6] << 16) + (datagram[7] << 8) + datagram[8];
                    string device;
                    switch (kenn)
                    {
                        case 0x0010: device = "Gleisbox"; break;
                        case 0x0030: device = "Mobile Station 2"; break;
                        case 0x0033: device = "Mobile Station 2"; break;
                        case 0xFFFF: device = "CS2-GUI (Master)"; break;
                        case 0x1234: device = "MäCAN-Decoder"; break;
                        default: device = "unbekannt"; break;
                    }
                    Console.Write("Bootloader-Antwort von {0} (0x{1:x4}) mit UID 0x{2:X8}, Software-Version {3}.{4}", device, kenn, uid, datagram[9], datagram[10]);

                }
                if (cmdIndex == 7 && !cmdResp) Console.Write("Bootloader-Anfrage");

                //Config:
                if (cmdIndex == 8 && !cmdResp)
                {
                    int uid = (datagram[5] << 24) + (datagram[6] << 16) + (datagram[7] << 8) + datagram[8];
                    Console.Write("Config-Anfrage an UID 0x{0:X8}, Kanal {1}", uid, datagram[9]);
                }
                if (cmdIndex == 8 && cmdResp && datagram[4] == 6)
                {
                    int uid = (datagram[5] << 24) + (datagram[6] << 16) + (datagram[7] << 8) + datagram[8];
                    Console.Write("Config-Antwort von UID 0x{0:X8}, Kanal {1}, {2} Pakete gesendet", uid, datagram[9], datagram[10]);
                }
                else if (cmdIndex == 8 && cmdResp)
                {
                    Console.Write("Paket {0}", hash - 0x300);
                }
            }
            else Console.Write("   CS1-PROTOKOLL!");
        }

        static string getLoco(byte[] datagram)
        {

            int locID = (datagram[7] << 8) + datagram[8];
            string prot;
            int addrs;

            if (locID >= 0x0000 && locID <= 0x03ff)
            {
                prot = "Motorola";
                addrs = locID;
            }
            else if (locID >= 0xC000 && locID <= 0xFFFF)
            {
                prot = "DCC";
                addrs = locID - 0xc000;
            }
            else
            {
                prot = "unbekannt";
                addrs = 0;
            }
            return prot + "-" + addrs;
        }

        static string getAcc(byte[] datagram)
        {

            int locID = (datagram[7] << 8) + datagram[8];
            string prot;
            int addrs;

            if (locID >= 0x3000 && locID <= 0x33ff)
            {
                prot = "Motorola";
                addrs = locID - 0x3000 + 1;
            }
            else if (locID >= 0x3800 && locID <= 0x3FFF)
            {
                prot = "DCC";
                addrs = locID - 0x3800 + 1;
            }
            else
            {
                prot = "unbekannt";
                addrs = 0;
            }
            return prot + "-" + addrs;
        }

        static void writeRed(string str)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write(str);
            Console.ResetColor();
        }

        static void writeGreen(string str)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write(str);
            Console.ResetColor();
        }

    }
}
