window.onload = function() {
    setModeRequest(0);
}

$('#modeSelect').change(function(){
    let modeIndex = $(this).val();
    if(modeIndex == 0) {
        enableDrawing();
    } else {
        disableDrawing();
    }

    if(modeIndex == 8)
        setAnimIntervalRequest(10);
    else
        setAnimIntervalRequest(100)

    setModeRequest(modeIndex);
})

function setModeRequest(modeIndex) {
    $.post(lampUrl + '/setMode', JSON.stringify({modeIndex: modeIndex}))
            .done(function(response){
                console.log(response);
            });
}

function setAnimIntervalRequest(animInterval) {
    $.post(lampUrl + '/setAnimInterval', JSON.stringify({animInterval: animInterval}))
            .done(function(response){
                console.log(response);
            });
}