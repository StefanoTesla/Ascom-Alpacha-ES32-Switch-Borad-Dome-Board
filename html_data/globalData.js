export default function GlobalData(text) {
    return {
        text: text,
        exist: {},
        dome:{},
        open:  {switch:false,dome:false,cover:false,statistics:false},
        dataLoaded :false,
    


    init(){

        // get the defined boards
        fetch('http://192.168.222.235/api/cfg')
        .then(response => response.json())
        .then(data => {
            this.exist = data.define;
            this.getDomeStatus()
        })
        .catch(error => console.error('Error fetching board data:', error));
    },



    getDomeStatus(){
        fetch('http://192.168.222.235/api/dome/status')
        .then(response => response.json())
        .then(data => {
            this.dome = data;
            this.dataLoaded = true;
        })
        .catch(error => console.error('Error fetching board data:', error));
    }



    };
}