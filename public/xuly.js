var socket = io("http://localhost:6060");

socket.on("server-update-data", function (data) {
	// Home page
	$("#currentTemp").html(data.temp);
	$("#currentHumi").html(data.humi);
	$("#currentLight").html(data.light);

	// Warning mode
	var warningSection = document.getElementById("warningSection");
	if (data.temp > 40 || data.humi > 80) {
		warningSection.classList.add("warning-mode-on");
	} else {
		warningSection.classList.remove("warning-mode-on");
	}
	//History page
	$("#id-content").append("<div class='h-para'>" + data.id + "</div>");
	$("#time-content").append("<div class='h-para'>" + data.time + "</div>");
	$("#temp-content").append("<div class='h-para'>" + data.temp + "</div>");
	$("#humi-content").append("<div class='h-para'>" + data.humi + "</div>");
	$("#light-content").append("<div class='h-para'>" + data.light + "</div>");
});

socket.on("send-full", function (data) {
	// History page
	$("#light-content").html("");
	$("#time-content").html("");
	$("#temp-content").html("");
	$("#humi-content").html("");
	$("#id-content").html("");

	// console.log(data)
	data.forEach(function (item) {
		$("#light-content").append(
			"<div class='h-para'>" + item.light + "</div>"
		);
		$("#time-content").append("<div class='h-para'>" + item.time + "</div>");
		$("#temp-content").append("<div class='h-para'>" + item.temp + "</div>");
		$("#humi-content").append("<div class='h-para'>" + item.humi + "</div>");
		$("#id-content").append("<div class='h-para'>" + item.id + "</div>");
	});
});

// ---- Control devices ----
function livingroomLight() {
	var checkBox = document.getElementById("livingroomLight");
	if (checkBox.checked == true) {
		//alert("LED On");
		document.getElementById("livingrom-light").classList.remove("turn-off");
		document.getElementById("livingrom-light").classList.add("turn-on");
		console.log(document.getElementById("livingrom-light").classList);
		socket.emit("livingroomLightChange", "on");
	} else {
		//alert("LED Off");
		document.getElementById("livingrom-light").classList.remove("turn-on");
		document.getElementById("livingrom-light").classList.add("turn-off");
		console.log(document.getElementById("livingrom-light").classList);
		socket.emit("livingroomLightChange", "off");
	}
}

function livingroomAirConditioner() {
	var checkBox = document.getElementById("livingroomAirConditioner");
	if (checkBox.checked == true) {
		//alert("LED On");
		document.getElementById("livingroom-air-conditioner").classList.remove("turn-off");
		document.getElementById("livingroom-air-conditioner").classList.add("turn-on");
		console.log(document.getElementById("livingroom-air-conditioner").classList);
		socket.emit("livingroomAirConditionerChange", "on");
	} else {
		//alert("LED Off");
		document.getElementById("livingroom-air-conditioner").classList.remove("turn-on");
		document.getElementById("livingroom-air-conditioner").classList.add("turn-off");
		console.log(document.getElementById("livingroom-air-conditioner").classList);
		socket.emit("livingroomAirConditionerChange", "off");
	}
}

function television() {
	var checkBox = document.getElementById("television");
	if (checkBox.checked == true) {
		document.getElementById("television-container").classList.remove("turn-on");
		document.getElementById("television-container").classList.add("turn-off");
		console.log(document.getElementById("television-container").classList);
		socket.emit("televisionChange", "on");
	} else {
		document.getElementById("television-container").classList.remove("turn-off");
		document.getElementById("television-container").classList.add("turn-on");
		console.log(document.getElementById("television-container").classList);
		socket.emit("televisionChange", "off");
	}
}

function bedroomLight() {
	var checkBox = document.getElementById("bedroomLight");
	if (checkBox.checked == true) {
		//alert('LED On')
		socket.emit("bedroomLightChange", "on");
	} else {
		// alert('LED Off')
		socket.emit("bedroomLightChange", "off");
	}
}

function bedroomAirConditioner() {
	var checkBox = document.getElementById("bedroomAirConditioner");
	if (checkBox.checked == true) {
		//alert('LED On')
		socket.emit("bedroomAirConditionerChange", "on");
	} else {
		// alert('LED Off')
		socket.emit("bedroomAirConditionerChange", "off");
	}
}

function airVent() {
	var checkBox = document.getElementById("airVent");
	if (checkBox.checked == true) {
		//alert('LED On')
		socket.emit("airVentChange", "on");
	} else {
		// alert('LED Off')
		socket.emit("airVentChange", "off");
	}
}

// ---- RTC ----