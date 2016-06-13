var url = "http://192.168.31.179:8080/?action=directionCommand";

$(document).ready(function() {
    "use strict";
    $("#up").click(function() {
        var direction = new Object();
        direction.LeftWheelSpeed = 100;
        direction.RightWheelSpeed = 100;

        var directionJson = JSON.stringify(direction);
        $.get(url,directionJson);
    });

    $("#down").click(function() {
        var direction = new Object();
        direction.LeftWheelSpeed = -100;
        direction.RightWheelSpeed = -100;

        var directionJson = JSON.stringify(direction);
        $.get(url,directionJson);
    });

    $("#left").click(function() {
        var direction = new Object();
        direction.LeftWheelSpeed = 0;
        direction.RightWheelSpeed = 100;

        var directionJson = JSON.stringify(direction);
        $.get(url,directionJson);
    });

    $("#right").click(function() {
        var direction = new Object();
        direction.LeftWheelSpeed = 100;
        direction.RightWheelSpeed = 0;

        var directionJson = JSON.stringify(direction);
        $.get(url,directionJson);
    });

});
