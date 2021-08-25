<div class="button_grid_2">
    <div id="button_edit" class="button" onclick="toggleEditMode()">Bearbeiten</div>
    <div id="button_add" class="button" onclick="addAccessory()">Hinzufügen</div>
</div>
<div class="keyboard_grid" id="keyboard_grid">
</div>

<div id="edit_frame">
    <div class="locolist_grid">
        <p>Name:</p>
        <input type="text" class="text_input" id="new_accessorie_name">
        
        <p>Adresse:</p>
        <input type="text" class="text_input" id="new_accessorie_adress">

        <p>Protokoll:</p>
        <select class="text_input" name="protocol" id="protocol_selector">
            <option value="dcc">DCC</option>
            <option value="mm">Motorola</option>
            <option value="MäCAN" disabled>MäCAN</option>
        </select>

        <p>Typ:</p>
        <select class="text_input" name="type" id="type_selector">
            <option value="standard">Standard</option>
            <option value="" disabled>Mehr in Arbeit...</option>
        </select>
    </div>
    <div class="button_grid_2">
        <div class="button power_button" onclick="closeEditFrame()">Abbrechen</div>
        <div class="button button_active" onclick="saveAccessorie()">Speichern</div>
    </div>
</div>

<script src="js/main.js" type="text/javascript"></script>
<script>
    var protocol = 0; //0 = DCC, 1 = Motorola
    var accessorys = {};
    var adding = false;
    var editing = false;


    /*for (let i = 1; i <= 1024; i++) {
        let acc_button = document.createElement('div');
        acc_button.className = 'acc button';
        let acc_button_content = `
            <div class="acc_indicator">
                <div onclick="setAcc(${i}, 0);"></div>
                <div></div>
                <div onclick="setAcc(${i}, 1);"></div>
            </div>
            <p class="acc_name">${i}</p>`
        acc_button.innerHTML = acc_button_content;
        keyboard_grid.appendChild(acc_button);
    }*/

    /*function setProtocol(_protocol) {
        if (!_protocol) {
            //Protokoll auf DCC setzen:
            button_dcc.className = "button button_active";
            button_mm.className = "button";
        } else {
            //Protokoll auf MM setzen:
            button_dcc.className = "button";
            button_mm.className = "button button_active";
        }

        protocol = _protocol;
    }*/

    function setAcc(uid, state, time) {
        //Magnetartikel bei klick schalten

        parent.ws.send(`setAcc:${uid}:${state}:${time}`);
    }

    function toggleEditMode() {
        if (!adding) {
            if (!editing) {
                editing = true;
                button_edit.className = 'button button_active';
            } else {
                editing = false;
                button_edit.className = 'button';
            }
        }
    }

    function addAccessory() {
        if (!editing) {
            adding = true;
            button_add.className = "button button_active";
            edit_frame.style.display = 'block';
        }
    }

    function closeEditFrame() {
        edit_frame.style.display = 'none';
        button_add.className = "button";
        adding = false;
    }

    function updateKeyboard(uid, state) {
        let _protocol;
        let adrs;
        let color = '#ff0025';
        if (state == '1') color = '#2fa938';

        if (uid >= 0x3000 && uid <= 0x33FF) {
            _protocol = 1;
            adrs = uid - 0x3000 + 1;
        } else if (uid >= 0x3800 && uid <= 0x3bFF) {
            _protocol = 0;
            adrs = uid - 0x3800 + 1
        }

        if (protocol == _protocol) document.getElementsByClassName('acc_indicator')[adrs - 1].children[1].setAttribute('style', 'background-color: ' + color + ';');

    }

    function updateKeyboard(keyboard) {
        for (let i = 0; i < keyboard.length; i++) {
            if (keyboard[i]) {
                let color = '#ff0025';
                if (keyboard[i].state == '1') color = '#2fa938';
                document.getElementsByClassName('acc_indicator')[i].children[1].setAttribute('style', 'background-color: ' + color + ';')
            }
        }
    }

    function loadKeyboard() {
        parent.ws.send('getKeyboard');
    }

    function saveAccessorie() {
        let new_accessorie = {};
        let uid;

        if (protocol_selector.value == "dcc") {
            uid = 0x3000 + parseInt(new_accessorie_adress.value);

        } else if (protocol_selector.value == "mm") {
            uid = 0x3800 +  parseInt(new_accessorie_adress.value);
        }

        new_accessorie.name = new_accessorie_name.value
        new_accessorie.uid = uid;
        new_accessorie.protocoll = protocol_selector.value;
        console.log(new_accessorie);
        parent.ws.send('addAccessorie:' + JSON.stringify(new_accessorie));
    }

    loadKeyboard();
</script>