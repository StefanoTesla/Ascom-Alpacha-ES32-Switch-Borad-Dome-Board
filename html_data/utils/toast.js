export function toast() {
    return {
        addToast(notice) {
              setTimeout(() => {
                notice.id = Date.now()
                this.notices.push(notice)
                this.fireToast(notice.id)
            }, 1 );  // put a delay to avoid toat crash
            /*notice.id = Date.now()
            this.notices.push(notice)
            
            this.fireToast(notice.id)
          */},
        fireToast(id) {
            this.visible.push(this.notices.find(notice => notice.id == id))
            const timeShown = 3000 * this.visible.length
            setTimeout(() => {
              this.removeToast(id)
            }, timeShown)
          },
        removeToast(id) {
            const notice = this.visible.find(notice => notice.id == id)
            const index = this.visible.indexOf(notice)
            this.visible.splice(index, 1)
          },
    
    }
}