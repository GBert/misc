<div class="button_grid_2">
    <div id="button_dcc" class="button button_active" onclick="setProtocol(0)">DCC</div>
    <div id="button_mm" class="button" onclick="setProtocol(1)">Motorola</div>
</div>
<div class="keyboard_grid" id="keyboard_grid">
    <!--<div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">1</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">2</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">3</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">4</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">5</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">6</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">7</p>
    </div>
    <div class="acc button">
        <div class="acc_indicator">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <p class="acc_name">8</p>
    </div>-->
</div>

<script src="js/main.js" type="text/javascript"></script>
<script>
    var protocol = 0; //0 = DCC, 1 = Motorola


    for (let i = 1; i <= 16; i++) {
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
    }

    function setProtocol(_protocol) {
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
    }

    function setAcc(adr, state) {
        //Magnetartikel bei klick schalten
    }
</script>