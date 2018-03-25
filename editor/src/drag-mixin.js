export default {
    data() {
        return { dragging: false };
    },
    methods: {
        drop(e) {
            this.dragging = false;
            e.stopPropagation();
            e.preventDefault();

            const items = Array.prototype.slice.call(e.dataTransfer.items);

            const that = this;
            // const files = e.dataTransfer.files;
            items.forEach((item) => {
                if (item.kind === "file") {
                    const reader = new FileReader();
                    reader.onloadend = function a() {
                        if (that.onDrop) {
                            that.onDrop(this.result);
                        }
                    };
                    reader.readAsText(item.getAsFile());
                }
            });
        },
        dragClear() {
            this.dragging = false;
        },
        dragEnter(e) {
            this.dragging = true;
            e.stopPropagation();
            e.preventDefault();
        },
        dragExit(e) {
            e.stopPropagation();
            e.preventDefault();
        },
        dragLeave(e) {
            e.stopPropagation();
            e.preventDefault();
        },
        dragOver(e) {
            e.stopPropagation();
            e.preventDefault();
            //               e.stopPropagation(); // e.preventDefault();
        }
    }
}
    ;
