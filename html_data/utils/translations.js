export function translations() {
    return {
        // Carica i dati dal file lang.json
        fetchTexts(locale) {
            const ip = import.meta.env.VITE_BOARD_IP
            fetch( locale + '.json')
                .then(response => response.json())
                .then(data => {
                    this.text = data;
                    this.textLoaded = true;
                })
                .catch(error => {
                    console.error('Error fetching translations');
                });
        },

    };
}