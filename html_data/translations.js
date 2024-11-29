export default function Translations() {
    return {
        locale:"",
        text: {},
        define: {},
        loaded:false,

        // get translations
        init() {
            //check if cookie with locale is stored
            //if is it, load the file (should be cached)
            //otherwise read the locale from the board and load the file
            if(this.getLanguageCookie()){
                this.fetchTexts();
            } else {
                this.getLocale();
            }
            
        },

        getLocale(){
            //override temporaneo
    //        this.locale = "it";
      //      this.setLanguageCookie(this.locale);
      //      this.fetchTexts();
            //override temporaneo
            fetch('http://localhost:3000/api/cfg')
                .then(response => response.json())
                .then(data => {
                    this.locale = data.locale;
                    this.setLanguageCookie(this.locale);
                    this.fetchTexts();
                })
                .catch(error => console.error('Error fetching translations:', error));
        },

        setLanguageCookie(language) {
            const expires = new Date();
            expires.setTime(expires.getTime() + (30 * 24 * 60 * 60 * 1000)); // Scade in 30 giorni
            document.cookie = `language=${language};expires=${expires.toUTCString()};path=/`;
        },

        getLanguageCookie() {
            const name = "language=";
            const decodedCookie = decodeURIComponent(document.cookie);
            const ca = decodedCookie.split(';');
            for (let i = 0; i < ca.length; i++) {
                let c = ca[i];
                while (c.charAt(0) === ' ') {
                    c = c.substring(1);
                }
                if (c.indexOf(name) === 0) {
                    this.locale = c.substring(name.length, c.length);
                    return c.substring(name.length, c.length);
                }
            }
            return null; // Se non è impostato il cookie
        },

        // Carica i dati dal file lang.json
        fetchTexts() {
            fetch('./'+this.locale+'.json')
                .then(response => response.json())
                .then(data => {
                    this.text = data;
                    this.loaded = true;
                })
                .catch(error => {
                    console.error('Error fetching translations: only en and it is supported at the moment');
                    this.deleteCookie();
                });
        },

        deleteCookie() {
            const expires = new Date();
            expires.setTime(expires.getTime() - 1); // Imposta la scadenza nel passato
            document.cookie = `language=;expires=${expires.toUTCString()};path=/`;
        }
    };
}