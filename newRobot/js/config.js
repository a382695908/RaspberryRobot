var url = "http://192.168.1.114:8080/?action=imageCommand";

$(document).ready(function() {
    "use strict";
    $("#submitButton").click(function() {
        var ImageValue = new Object();
        ImageValue.BRIGHTNESS = parseInt($("#brightness").val()) ;
        ImageValue.CONTRAST = parseInt($("#contrast").val());
        ImageValue.SATURATION = parseInt($("#saturation").val());
        //ImageValue.width = parseInt($("#width").val());
        ImageValue.WIDTH = 320;
        //ImageValue.height = parseInt($("#height").val());
        ImageValue.HEIGHT = 240;
        var ImageJson = JSON.stringify(ImageValue);
        //alert(ImageJson);

        $.get(url,ImageJson);
    });

});
