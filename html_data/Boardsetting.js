export default function BoardSetting(text) {
    return {
        text: text,
        exist: {},
        dome:{},
        domeOrig:{},
        coverC:{},
        coverCOrig:{},
        open:  {switch:false,dome:false,cover:false,statistics:false},
        dataLoaded :false,
        notices: [],
        visible: [],
        noUsablePin: [6,7,8,9,10,11,37,38],
        noUsableInputPin: [1],
        noUsableOutputPin: [3,34,35,36,39],
        warningPin:[0,3,5,12],
    


    init(){

        // get the defined boards
        fetch('http://192.168.222.235/api/cfg')
        .then(response => response.json())
        .then(data => {
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
        fetch('http://192.168.222.235/api/coverc/cfg')
        .then(response => response.json())
        .then(data => {

            this.coverCOrig = this.copy(data);
            this.coverC = data;
            this.dataLoaded = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    validateCoverC(){
        console.log("reactivity is my name...")
        let valid = true
        this.parseObjectToInt(this.coverC)

        if(this.coverC.calibrator.present){
            if(this.invalidOutputPin( this.coverC.calibrator.out )){ this.addValidationErrorClass("coverc_calibrator_out");  valid = false } else { this.removeValidationErrorClass("coverc_calibrator_out") }
        } else {
            this.removeValidationErrorClass("coverc_calibrator_out")
        }
        if(this.coverC.cover.present){
            if(this.invalidOutputPin( this.coverC.cover.out)){ this.addValidationErrorClass("coverc_cover_out");  valid = false } else { this.removeValidationErrorClass("coverc_cover_out") }
            if(this.negativeValue(this.coverC.cover.maxDeg) || this.coverC.cover.maxDeg>360){ this.addValidationErrorClass("coverc_cover_max_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_max_deg") }
            if(this.negativeValue(this.coverC.cover.closeDeg) || this.coverC.cover.closeDeg > this.coverC.cover.maxDeg){ this.addValidationErrorClass("coverc_cover_close_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_close_deg") }
            if(this.negativeValue(this.coverC.cover.openDeg) || this.coverC.cover.openDeg > this.coverC.cover.maxDeg){ this.addValidationErrorClass("coverc_cover_open_deg");  valid = false } else { this.removeValidationErrorClass("coverc_cover_open_deg") }
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
            fetch('http://192.168.222.235/api/coverc/cfg', {
                method: 'POST',
                headers: {
                  'Accept': 'application/json, text/plain, */*',
                  'Content-Type': 'application/json'
                },
                body: JSON.stringify(this.dome)
              }).then(res => res.json())
                .then(res => {
                    this.addToast({ type:"success", text: this.text.setting.coverc.configSaved })
                });
        }
    },


    /* dome area */

    getOriginalDomeConfig(){
        this.dome = this.copy(this.domeOrig)

    },

    getDomeConfig(){
        fetch('http://192.168.222.235/api/dome/cfg')
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
            fetch('http://192.168.222.235/api/dome/cfg', {
                method: 'POST',
                headers: {
                  'Accept': 'application/json, text/plain, */*',
                  'Content-Type': 'application/json'
                },
                body: JSON.stringify(this.dome)
              }).then(res => res.json())
                .then(res => {
                    this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                });
        }
    },

    validateDome(){
        console.log(this.dome.autoclose)
        let valid = true
        this.parseObjectToInt(this.dome)
        //input open
        if(this.invalidInputPin(this.dome.pinOpen.pin)){ this.addValidationErrorClass("dome_in_open"); valid = false} else { this.removeValidationErrorClass("dome_in_open")  }
        if(this.negativeValue( this.dome.pinOpen.dOn)){ this.addValidationErrorClass("dome_in_open_don");  valid = false } else { this.removeValidationErrorClass("dome_in_open_don") }
        if(this.negativeValue( this.dome.pinOpen.dOff)){ this.addValidationErrorClass("dome_in_open_doff");  valid = false } else { this.removeValidationErrorClass("dome_in_open_doff") }
        //input close
        if(this.invalidInputPin( this.dome.pinClose.pin)){ this.addValidationErrorClass("dome_in_close");  valid = false } else { this.removeValidationErrorClass("dome_in_close") }
        if(this.negativeValue( this.dome.pinClose.dOn)){ this.addValidationErrorClass("dome_in_close_don");  valid = false } else { this.removeValidationErrorClass("dome_in_close_don") }
        if(this.negativeValue( this.dome.pinClose.dOff)){ this.addValidationErrorClass("dome_in_close_doff");  valid = false } else { this.removeValidationErrorClass("dome_in_close_doff") }
        //outputs
        if(this.invalidOutputPin( this.dome.pinStart)){ this.addValidationErrorClass("dome_out_start");  valid = false } else { this.removeValidationErrorClass("dome_out_start") }
        if(this.invalidOutputPin( this.dome.pinHalt)){ this.addValidationErrorClass("dome_out_halt");  valid = false } else { this.removeValidationErrorClass("dome_out_halt") }
        //timers
        if(this.negativeValue( this.dome.movTimeOut)){ this.addValidationErrorClass("dome_timeout");  valid = false } else { this.removeValidationErrorClass("dome_timeout") }
        if(this.negativeValue( this.dome.autoclose.minutes)){ this.addValidationErrorClass("dome_autoclose_time");  valid = false } else { this.removeValidationErrorClass("dome_autoclose_time") }
        return valid
    
    },

    /* end dome */ 




    invalidOutputPin(pin){
        const noUsablePin = [6,7,8,9,10,11,20,24,28,29,30,31,37,38]
        const noUsableOutputPin= [3,34,35,36,39];
        if(noUsablePin.includes(pin) || noUsableOutputPin.includes(pin) || pin<0 || pin > 39){
            this.addToast({type:"error", text:"PIN " + pin + " "+ this.text.errors.noUsableAsOutput })
            return true
        }
        return false
    },

    invalidInputPin(pin){
        const noUsablePin = [6,7,8,9,10,11,20,24,28,29,30,31,37,38]
        const noUsableOutpuPin= [1];
        if(noUsablePin.includes(pin) || noUsableOutpuPin.includes(pin) || pin<0 || pin > 39){
            this.addToast({type:"error", text:"PIN " + pin + " "+ this.text.errors.noUsableAsInput })
            return true
        }
        return false
    },

    negativeValue(value){
        if(value<0){
            return true
        }
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