
export default function BoardSetting(text) {
    return {
        text: text,
        exist: {},
        dome:{},
        domeOrig:{},
        coverC:{},
        coverCOrig:{},
        board:{},
        open:  {switch:false,dome:false,cover:false,statistics:false},
        dataLoaded :false,
        notices: [],
        visible: [],
        noUsablePin: [6,7,8,9,10,11,37,38],
        noUsableInputPin: [1],
        noUsableOutputPin: [3,34,35,36,39],
        warningPin:[0,3,5,12],
    

    init(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/cfg')
        .then(response => response.json())
        .then(data => {
            this.board = data.board
            this.exist = data.define;
            if(this.exist.dome){
                this.getDomeConfig()
            }
            if(this.exist.coverc){
                this.getCoverCConfig()
            }
            this.dataLoaded = true;
            
        })
        .catch(error => console.error('Error fetching board data:', error));
    },


    /* coverC config */

    getOriginalCoverCConfig(){
        this.coverC = this.copy(this.coverCOrig)

    },


    getCoverCConfig(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip +'/api/coverc/cfg')
        .then(response => response.json())
        .then(data => {

            this.coverCOrig = this.copy(data);
            this.coverC = data;
            this.dataLoaded = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    validateCoverC(){
        let valid = true
        this.parseObjectToInt(this.coverC)

        if(this.coverC.calibrator.present){
            if(this.invalidOutputPin( this.coverC.calibrator.out, "coverc_calibrator_out" )){ valid = false }
        } else {
            this.removeValidationErrorClass("coverc_calibrator_out")
        }
        if(this.coverC.cover.present){
            if(this.invalidOutputPin( this.coverC.cover.out,"coverc_cover_out")){ valid = false }
            if(this.negativeValue(this.coverC.cover.maxDeg)){ valid = false }
            if(this.negativeValue(this.coverC.cover.closeDeg)){ valid = false }
            if(this.negativeValue(this.coverC.cover.openDeg) ){ valid = false }
            if(this.coverC.cover.maxDeg>360) { this.addValidationErrorClass("coverc_cover_max_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_max_deg") }
            if(this.coverC.cover.closeDeg > this.coverC.cover.maxDeg){ this.addValidationErrorClass("coverc_cover_close_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_close_deg") }
            if( this.coverC.cover.openDeg > this.coverC.cover.maxDeg){ this.addValidationErrorClass("coverc_cover_open_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_open_deg") }
        } else {
            this.removeValidationErrorClass("coverc_cover_out")
            this.removeValidationErrorClass("coverc_cover_max_deg")
            this.removeValidationErrorClass("coverc_cover_close_deg")
            this.removeValidationErrorClass("coverc_cover_open_deg")
        }
        return valid
    
    },

    saveCoverCSetting(){
        if(this.validateCoverC()){
            const ip = import.meta.env.VITE_BOARD_IP
            fetch(ip + '/api/coverc/cfg', {
                method: 'POST',
                headers: {
                  'Accept': 'application/json, text/plain, */*',
                  'Content-Type': 'application/json'
                },
                body: JSON.stringify(this.coverC)
              }).then(res => {
                // Controllo dello stato HTTP
                if (!res.ok) {
                    // Analizzo la risposta JSON anche per errori 500
                    return res.json().then(errorResponse => {
                        throw { status: res.status, ...errorResponse };
                    });
                }
                return res.json();
            })
            .then(res => {
                this.addToast({ type: "success", text: this.text.setting.coverC.configSaved });
            })
            .catch(err => {
                if (err.errors) {
                    err.errors.forEach((error, index) => {
                        setTimeout(() => {
                            console.log(`Errore ${index + 1}: ${error}`);
                            this.addToast({ type: "error", text: `Errore: ${error}` });
                        }, 1 * index);  // put a delay to avoid toat crash
                    });
                } else {
                    console.log("Errore sconosciuto:", err);
                    this.addToast({ type: "error", text: "Errore sconosciuto." });
                }
            });
        }
    },


    /* dome area */

    getOriginalDomeConfig(){
        this.dome = this.copy(this.domeOrig)

    },

    getDomeConfig(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/dome/cfg')
        .then(response => response.json())
        .then(data => {

            this.domeOrig = this.copy(data);
            this.dome = data;
            this.dataLoaded = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },


    saveDomeSetting(){
        if(this.validateDome()){
            const ip = import.meta.env.VITE_BOARD_IP
            fetch(ip + '/api/dome/cfg', {
                method: 'POST',
                headers: {
                  'Accept': 'application/json, text/plain, */*',
                  'Content-Type': 'application/json'
                },
                body: JSON.stringify(this.dome)
              }).then(res => res.json())
                .then(res => {
                    this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                })
                .catch(err => {
                    try {
                        const errorData = JSON.parse(err.message);
                        console.log("Errors:", errorData.errors);
                        this.addToast({ type: "error", text: "Errore: " + errorData.errors.join(", ") });
                    } catch (parseError) {
                        console.log("Errore sconosciuto:", err);
                        this.addToast({ type: "error", text: "Errore sconosciuto." });
                    }
                });
        }
    },

    validateDome(){
        console.log(this.dome.autoclose)
        let valid = true
        this.parseObjectToInt(this.dome)
        //input open
        if(this.invalidInputPin(this.dome.pinOpen.pin,"dome_in_open")){ valid = false}
        if(this.negativeValue( this.dome.pinOpen.dOn,"dome_in_open_don")){ valid = false }
        if(this.negativeValue( this.dome.pinOpen.dOff,"dome_in_open_doff")){ valid = false }
        //input close
        if(this.invalidInputPin( this.dome.pinClose.pin,"dome_in_close")){ valid = false }
        if(this.negativeValue( this.dome.pinClose.dOn,"dome_in_close_don")){ valid = false }
        if(this.negativeValue( this.dome.pinClose.dOff,"dome_in_close_doff")){ valid = false }
        //outputs
        if(this.invalidOutputPin( this.dome.pinStart,"dome_out_start")){ valid = false }
        if(this.invalidOutputPin( this.dome.pinHalt,"dome_out_halt")){ valid = false }
        //timers
        if(this.negativeValue( this.dome.movTimeOut,"dome_timeout")){ this.addValidationErrorClass();  valid = false }
        if(this.negativeValue( this.dome.autoclose.minutes,"dome_autoclose_time")){ this.addValidationErrorClass();  valid = false }
        return valid
    
    },

    /* end dome */ 


    /* board config */

    saveBoardConfig(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/board/cfg', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
              'Content-Type': 'application/json'
            },
            body: JSON.stringify(this.board)
        }).then(res => res.json())
        .then(res => {
            this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
        })
        .catch(err => {
                try {
                    const errorData = JSON.parse(err.message);
                    console.log("Errors:", errorData.errors);
                    this.addToast({ type: "error", text: "Errore: " + errorData.errors.join(", ") });
                } catch (parseError) {
                    console.log("Errore sconosciuto:", err);
                    this.addToast({ type: "error", text: "Errore sconosciuto." });
                }
        })
        .finally(() => {
            this.deleteCookie();
        });

    },


    deleteCookie() {
        const expires = new Date();
        expires.setTime(expires.getTime() - 1); // Imposta la scadenza nel passato
        document.cookie = `language=;expires=${expires.toUTCString()};path=/`;
    },

    invalidOutputPin(pin,divclass){
        const noUsablePin = [6,7,8,9,10,11,20,24,28,29,30,31,37,38]
        const noUsableOutputPin= [3,34,35,36,39];
        if(noUsablePin.includes(pin) || noUsableOutputPin.includes(pin) || pin<0 || pin > 39){
            this.addToast({type:"error", text:"PIN " + pin + " "+ this.text.errors.noUsableAsOutput })
            this.addValidationErrorClass(divclass);
            return true
        }
        this.removeValidationErrorClass(divclass);
        return false
    },

    invalidInputPin(pin,divclass){
        const noUsablePin = [6,7,8,9,10,11,20,24,28,29,30,31,37,38]
        const noUsableOutpuPin= [1];
        if(noUsablePin.includes(pin) || noUsableOutpuPin.includes(pin) || pin<0 || pin > 39){
            this.addToast({type:"error", text:"PIN " + pin + " "+ this.text.errors.noUsableAsInput })
            this.addValidationErrorClass(divclass);
            return true
        }
        this.removeValidationErrorClass(divclass);
        return false
    },

    negativeValue(value,divclass){
        if(value<0){
            return true
            this.addValidationErrorClass(divclass);
        }
        this.removeValidationErrorClass(divclass);
        return false
    },

    addValidationErrorClass(id){
        const el = document.getElementById(id);
        if (el) {el.classList.add('validation_error');
                el.scrollIntoView({ behavior: 'smooth', block: 'center' });}
    },

    removeValidationErrorClass(id){
        const el = document.getElementById(id);
        if (el) el.classList.remove('validation_error');
    },



    /* used to store loaded configuration*/
    copy(object){
        return JSON.parse(JSON.stringify(object));
    },

    parseObjectToInt(obj){
        for (let key in obj) {
            if (obj.hasOwnProperty(key)) {
                if (typeof obj[key] === 'object' && obj[key] !== null) {
                    // Se è un oggetto, chiamare ricorsivamente
                    this.parseObjectToInt(obj[key]);
                } else {
                    // Se è un valore primitivo (numero o stringa), applica parseInt
                    if(typeof obj[key] !== 'boolean' ){
                        obj[key] = parseInt(obj[key], 10);
                    }
                   
                }
            }
        }
    },

    //toast


    addToast(notice) {
        notice.id = Date.now()
        this.notices.push(notice)
        this.fireToast(notice.id)
      },
    fireToast(id) {
        this.visible.push(this.notices.find(notice => notice.id == id))
        const timeShown = 3000 * this.visible.length
        setTimeout(() => {
          this.removeToast(id)
        }, timeShown)
      },
    removeToast(id) {
        const notice = this.visible.find(notice => notice.id == id)
        const index = this.visible.indexOf(notice)
        this.visible.splice(index, 1)
      },


    };
}