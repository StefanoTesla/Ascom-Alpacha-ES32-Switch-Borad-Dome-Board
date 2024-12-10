import { toast } from './utils/toast.js'
import { validation } from './utils/validation.js'
import { translations } from './utils/translations.js'
import { dome } from './utils/dome.js'
import { coverc } from './utils/coverc.js'
import { switches } from './utils/swtiches.js'

export default function GlobalData() {
    return {
        textLoaded:false,
        exist: {},
        dome:{},
        coverC:{},
        board:{},
        open:  {switch:false,dome:false,cover:false,statistics:false},
        manual:{
            cover:0,
            brightness:0,
        },
        dataLoaded :false,
        notices: [],
        visible: [],

    init(){
        const ip = import.meta.env.VITE_BOARD_IP
        // get the defined boards
        fetch(ip+'/api/cfg')
        .then(response => response.json())
        .then(data => {
            this.fetchTexts(data.locale);
            this.exist = data.define;
            this.updateData()
            this.updateBoard()
            
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
        this.dataLoaded = true;
        setTimeout(() => {this.updateData()}, 3000)
      },

    updateBoard(){
        const ip = import.meta.env.VITE_BOARD_IP
        fetch(ip+'/api/board/status')
        .then(response => response.json())
        .then(data => {
            this.board = data;
            
        })
        .catch(error => console.error('Error fetching board data:', error));

        setTimeout(() => {this.updateBoard()}, 30000)
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



    ...translations(),
    ...validation(),
    ...toast(),
    ...dome(),
    ...coverc(),
    ...switches()

    };
}