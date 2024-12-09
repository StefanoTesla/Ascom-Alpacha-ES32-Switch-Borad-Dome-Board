export function coverc(){
    return {
    
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
            this.load.cover = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    validateCoverC(){
        let valid = true
        this.parseObjectToInt(this.coverC)

        if(this.coverC.calibrator.present){
            console.debug(this.coverC.calibrator.out)
            if(this.invalidOutputPin( this.coverC.calibrator.out, "coverc_calibrator_out" )){ valid = false }
        } else {
            this.removeValidationErrorClass("coverc_calibrator_out")
        }
        if(this.coverC.cover.present){
            console.debug(this.coverC.cover)
            if(this.invalidOutputPin(this.coverC.cover.out,"coverc_cover_out")){ valid = false }
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
    }

    }
}