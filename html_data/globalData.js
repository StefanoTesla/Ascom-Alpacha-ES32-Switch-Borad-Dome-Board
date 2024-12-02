export default function GlobalData(text) {
    return {
        text: text,
        exist: {},
        dome:{},
        coverC:{},
        open:  {switch:false,dome:false,cover:false,statistics:false},
        manual:{
            cover:0,
            brightness:0,
        },
        dataLoaded :false,
    


    init(){
        const ip = import.meta.env.VITE_BOARD_IP
        // get the defined boards
        fetch(ip+'/api/cfg')
        .then(response => response.json())
        .then(data => {
            this.exist = data.define;
                this.updateData()
                this.dataLoaded = true;
        })
        .catch(error => console.error('Error fetching board data:', error))
    },

    updateData(){
        if (this.exist.dome){
            this.getDomeStatus()
        }
        if (this.exist.coverc){
            this.getCoverCStatus()
        }
        setTimeout(() => {this.updateData()}, 3000)
      },


    getDomeStatus(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip+'/api/dome/status')
        .then(response => response.json())
        .then(data => {
            this.dome = data;
            
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    getCoverCStatus(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip+'/api/coverc/status')
        .then(response => response.json())
        .then(data => {
            this.coverC = data;
        })
        .catch(error => console.error('Error fetching board data:', error));
    },

    coverClose(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/coverc/close', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
            },
          }).then(res => res.json())
            .then(res => {
                //this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                console.log(res)
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
    },
    coverOpen(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/coverc/open', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
            },
          }).then(res => res.json())
            .then(res => {
                //this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                console.log(res)
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
    },


    calibratorBrightness(value){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/coverc/brightness', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
              'Content-Type': 'application/x-www-form-urlencoded'
            },
            body:"brightness="+value
          }).then(res => res.json())
            .then(res => {
                //this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                console.log(res)
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
    },

    calibratorOff(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/coverc/off', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
            },
          }).then(res => res.json())
            .then(res => {
                if (res.execute){
                    this.coverC.calibrator.brightness = 0;
                }
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
    },

    calibratorOn(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/coverc/on', {
            method: 'POST',
            headers: {
              'Accept': 'application/json, text/plain, */*',
            },
          }).then(res => res.json())
            .then(res => {
                //this.addToast({ type:"success", text: this.text.setting.dome.configSaved })
                if (res.execute){
                    this.coverC.calibrator.brightness = 4096;
                }
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




    };
}