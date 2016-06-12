// document.getElementById("submitButton").onclick(submitForm);

// function submitForm() {
//     document.forms["login"].submit();
// }


$(document).ready(function(){
"use strict";
    $("#submitButton").click(function(){
        var user=$("#user").val();
        var pwd=$("#password").val();
        if(user== "root" && pwd=="fuck"){
            window.location.replace("main.html");
        }
        else{
            alert("用户名或账号不存在");
            window.location.reload();
        }
    });


});