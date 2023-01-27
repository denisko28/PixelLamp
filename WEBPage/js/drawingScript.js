var picker = $(".picker").colorPick({
    'onColorSelected': function() {
        document.getElementById("gridContainer").setAttribute("paintColor", this.color);
    } 
});

function disableDrawing() {
    let drawingField = document.getElementById("drawingField");
    drawingField.setAttribute("class", "disabled");

    let clearButton = document.getElementById("clearDrawingButt");
    clearButton.setAttribute("class", "disabled");
}

function enableDrawing() {
    let drawingField = document.getElementById("drawingField");
    drawingField.removeAttribute("class");

    let clearButton = document.getElementById("clearDrawingButt");
    clearButton.removeAttribute("class");
}