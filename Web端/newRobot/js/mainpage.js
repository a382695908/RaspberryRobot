var ON=1;
var OFF=0;
var rasp_button_state=ON;
var friend_button_state=OFF;
var add_button_state=OFF;
var control_button_state=OFF;
$(document).ready(function(){
"use strict";
    $("#rasp_button").click(function() {
        if(friend_button_state==ON){
            // $("#friends").fadeOut('fast');
             $("#friends").css("display","none");
            friend_button_state=OFF;
        }
        else if(add_button_state==ON){
            $("#add").css("display","none");
            add_button_state=OFF;
        }
        if(rasp_button_state==OFF){
            $("#rasp_button:after").css("display","block");
            $("#cars").fadeIn(1200);
            rasp_button_state=ON;
        }
    });
    $("#friend_button").click(function() {
        if(rasp_button_state==ON){
            $("#cars").css("display","none");
            rasp_button_state=OFF;
        }
        else if(add_button_state==ON){
            $("#add").css("display","none");
            add_button_state=OFF;
        }
        if(friend_button_state==OFF){
            $("#friends").fadeIn(1200);
            friend_button_state=ON;
        }
    });
    $("#add_button").click(function() {
        if(friend_button_state==ON){
            $("#friends").css("display","none");
            friend_button_state=OFF;
        }
        else if(rasp_button_state==ON){
            $("#cars").css("display","none");
            rasp_button_state=OFF;
        }
        if(add_button_state==OFF){
            $("#add").fadeIn(1200);
            add_button_state=ON;
        }
    });
	/*
	$("a.control_button").click(function(){
		if(control_button_state ==OFF){
			//$("#pi_show_img").attr("src","http://h15121i262.imwork.net/?action=stream");
			//$("#pi_show_img").attr("src","http://192.168.31.179:8080/?action=stream");
			$("#control_str").html("停止控制");
			control_button_state =ON;
		}
		else{
			//$("#pi_show_img").attr("src","images/raspberry_pi.png");
			//$("#pi_show img").css("display","block");
			$("#control_str").html("开始控制");
			control_button_state =OFF;
		}
	});*/
});
