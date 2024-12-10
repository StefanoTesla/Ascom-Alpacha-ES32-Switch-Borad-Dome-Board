export function dome(){
    return {

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
            this.load.dome = true;
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

    
    getDomeStatus(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip+'/api/dome/status')
        .then(response => response.json())
        .then(data => {
            this.dome = data;
            this.load.dome = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },



    }
}