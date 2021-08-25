In dieses Verzeichnis aus dem Archiv MaeCAN-Server die folgenden Dateien bzw.
Verzeichnisse kopieren:

- html/js
- node
- html/php
- html/styles
- html/index.php
- html/integrator.php

node.js installieren
apt-get install nodejs
apt-get install npm
npm -g install forever

ln -s /usr/bin/nodejs /usr/bin/node

nach installtion des init scripts:
update-rc.d my-application defaults
