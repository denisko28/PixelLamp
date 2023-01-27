const stripLengthes = [2, 6, 8, 8, 10, 10, 8, 8, 6, 2];

/* EventListener for drawing */
$(document).ready(function () {
  /* Used to reduce redundant html grid div code by using a nested loop to create the div elements in the container which is a 16x16 grid */
  for (let i = 0; i < stripLengthes.length; i++) {
    let newRow = document.createElement("div");
    let rowAttr = document.createAttribute("class");
    rowAttr.value = "row";
    newRow.setAttributeNode(rowAttr);
    document.getElementById("gridContainer").appendChild(newRow);
    for (let j = 0; j < stripLengthes[i]; j++) {
      let div = document.createElement("button");
      let attr = document.createAttribute("class");
      attr.value = "grid";
      div.setAttributeNode(attr);
      newRow.appendChild(div);
    }
  }

  var pressedDown = false;
  $("#gridContainer")
    .bind("mousedown touchstart", function () {
      pressedDown = true; // When mouse goes down, set pressedDown to true
    })
    .bind("mouseup touchend", function () {
      pressedDown = false; // When mouse goes up, set pressedDown to false
      sendArrayToLamp();
    })
    .bind("mousemove touchmove", function (e) {
      if (pressedDown) {
        var touch = e.touches[0];
        let ele = document.getElementsByClassName("grid");
        for (let i = 0; i < ele.length; i++) {
          var grid = ele[i];
          if(touch.clientY >= grid.offsetTop &&  touch.clientY <= grid.offsetTop + grid.offsetHeight)
            if(touch.clientX >= grid.offsetLeft &&  touch.clientX <= grid.offsetLeft + grid.offsetWidth) {
              ele[i].style.backgroundColor = $("#gridContainer").attr("paintColor");
            }
        };
      }
    });
  $('.grid').bind("mousedown touchstart", function(){
    $(this).css({'backgroundColor': $("#gridContainer").attr("paintColor")});
  });
});

/****** Function is used to clear the board ******/
function clearGrid() {
  let ele = document.getElementsByClassName("grid");
  for (let i = 0; i < ele.length; i++) {
    ele[i].style.backgroundColor = "";
  }
}

const rgb2hex = (rgb) => `0x${rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/).slice(1).map(n => parseInt(n, 10).toString(16).padStart(2, '0')).join('')}`

function sendArrayToLamp() {
  var array = [];
  var testArray = [];
  let ele = document.getElementsByClassName("grid");
  for (let i = 0; i < ele.length; i++) {
    var color = ele[i].style.backgroundColor;
    array.push(color ? parseInt(rgb2hex(color), 16) : 0);
    testArray.push(color ? rgb2hex(color) : "0x000000");
  }
  //console.log(array);
  var strArray = testArray.toString();
  strArray.replace("'", "");
  console.log(`[${strArray}]`);

  $.post(lampUrl + '/drawPicture', JSON.stringify({array: array})).done(function(response){
      console.log(response);
  });
}
