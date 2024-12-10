export function switches(){
    return{

    
    /* switch config */
    getSwitchConfig(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip +'/api/switch/cfg')
        .then(response => response.json())
        .then(data => {

            this.switchCOrig = this.copy(data);
            this.swi = data;
            this.load.switch = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    getOriginalSwitchesConfig(){
        this.swi = this.copy(this.switchCOrig)
    },

    addNewSwitch(){
        if(this.swi.Switches.length <= 14){
            this.swi.Switches.push({"name":"","desc":"","type":0,"pin":0,"min":0,"max":0})
        } else {
            this.addToast({type:"error", text:"Limite Raggiunto" })
        }
        
    },

    deleteSwitch(id){
        this.swi.Switches.splice(id,1)
    },

    moveUp(id){
        [this.swi.Switches[id], this.swi.Switches[id - 1]] = [this.swi.Switches[id - 1], this.swi.Switches[id]];
    },

    moveDown(id){
        [this.swi.Switches[id], this.swi.Switches[id + 1]] = [this.swi.Switches[id + 1], this.swi.Switches[id]];
    },

    validateSwitch(index){
        this.swi.Switches[index].name = this.swi.Switches[index].name.replace(/[@#$*<>:;!]/g, '')
        this.swi.Switches[index].desc = this.swi.Switches[index].desc.replace(/[@#$*<>:;!]/g, '')
        this.swi.Switches[index].type = parseInt(this.swi.Switches[index].type)
        this.swi.Switches[index].pin = parseInt(this.swi.Switches[index].pin)
        this.swi.Switches[index].min = parseInt(this.swi.Switches[index].min)
        this.swi.Switches[index].max = parseInt(this.swi.Switches[index].max)

        switch (this.swi.Switches[index].type) {
            case 0:
                return true;
                break;
            case 1: //di
            case 2: //ai
                if(this.invalidInputPin(this.swi.Switches[index].pin,'sw_'+ index +'_pin')){ return false }
                return true
                break;
            case 4://pwm
            case 5://servo
            case 6://ao
                if(this.swi.Switches[index].max <= this.swi.Switches[index].min){
                    this.addValidationErrorClass('sw_'+ index +'_max')
                    return false
                } else {
                    this.removeValidationErrorClass('sw_'+ index +'_max')
                }
                if(this.swi.Switches[index].min >= this.swi.Switches[index].max){
                    this.addValidationErrorClass('sw_'+ index +'_min')
                    return false
                } else {
                    this.removeValidationErrorClass('sw_'+ index +'_min')
                }
            case 3://do
                if(this.invalidOutputPin(this.swi.Switches[index].pin,'sw_'+ index +'_pin')){ return false }
                return true
                break;

            default:
                // codice da eseguire se nessun case corrisponde
                break;
        }

    },



    saveSwitchesSetting(){
  
    let error = false;
        this.swi.Switches.forEach((element,index) => {
            if(!this.validateSwitch(index)){
                error = true;
            }
        });
    if(error){
        return;
    }

    const ip = import.meta.env.VITE_BOARD_IP
    fetch(ip + '/api/switch/cfg', {
        method: 'POST',
        headers: {
          'Accept': 'application/json, text/plain, */*',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(this.swi)
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
    



    }




}