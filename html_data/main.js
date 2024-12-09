import './style.css'
import Alpine from 'alpinejs'
import Transaltions from './translations'
import GlobalData from './globalData'
import BoardSetting from './Boardsetting'
import Switches from './Switches'

window.Alpine = Alpine
window.Translations = Transaltions;  // Rendi la funzione globale
window.GlobalData = GlobalData;
window.BoardSetting = BoardSetting;
Alpine.start()

