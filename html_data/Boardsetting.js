import { toast } from './utils/toast.js'
import { validation } from './utils/validation.js'
import { translations } from './utils/translations.js'
import { dome } from './utils/dome.js'
import { coverc } from './utils/coverc.js'
import { switches } from './utils/swtiches.js'

export default function BoardSetting() {
    return {
        textLoaded:false,
        exist: {switch:true},
        dome:{},
        domeOrig:{},
        coverC:{},
        coverCOrig:{},
        swi:{},
        switchOrig:{},
        board:{},
        open:{switch:false,dome:false,cover:false,statistics:false},
        load:{switch:false,dome:false,cover:false,statistics:false},
        notices: [],
        visible: [],

    init(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip + '/api/cfg')
        .then(response => response.json())
        .then(data => {
            this.fetchTexts(data.locale);
            this.board = data.board
            this.exist = data.define;

            if(this.exist.dome){
                this.getDomeConfig()
            }
            if(this.exist.coverc){
                this.getCoverCConfig()
            }
            if(this.exist.switch){
                this.getSwitchConfig()
            }
            
        })
        .catch(error => console.error('Error fetching board data:', error));
    },



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



    /* used to store loaded configuration*/
    copy(object){
        return JSON.parse(JSON.stringify(object));
    },


    ...translations(),
    ...validation(),
    ...toast(),
    ...dome(),
    ...coverc(),
    ...switches()

    };
}