// Set up mouse events for drawing
var mouse = { x:0, y:0, isDown:false };

canvas.addEventListener("mousedown", function (e) {
	var mousePos = getMousePos(canvas, e);
	mouse.x=mousePos.x;
	mouse.y=mousePos.y;
	mouse.isDown=true;
}, false);

canvas.addEventListener("mouseup", function (e) {
	var mousePos = getMousePos(canvas, e);
	mouse.x=mousePos.x;
	mouse.y=mousePos.y;
	mouse.isDown=false;
}, false);

canvas.addEventListener("mousemove", function (e) {
	var mousePos = getMousePos(canvas, e);
	mouse.x=mousePos.x;
	mouse.y=mousePos.y;
}, false);

// Set up touch events for mobile, etc
canvas.addEventListener("touchstart", function (e) {
	mousePos = getTouchPos(canvas, e);
	var touch = e.touches[0];
	var mouseEvent = new MouseEvent("mousedown", {
		clientX: touch.clientX,
		clientY: touch.clientY
	});
	canvas.dispatchEvent(mouseEvent);
}, false);

canvas.addEventListener("touchend", function (e) {
	var mouseEvent = new MouseEvent("mouseup", {});
	canvas.dispatchEvent(mouseEvent);
}, false);

canvas.addEventListener("touchmove", function (e) {
	var touch = e.touches[0];
	var mouseEvent = new MouseEvent("mousemove", {
		clientX: touch.clientX,
		clientY: touch.clientY
	});
	canvas.dispatchEvent(mouseEvent);
}, false);

// Prevent scrolling when touching the canvas
document.body.addEventListener("touchstart", function (e) {
	if (e.target == canvas) {
		e.preventDefault();
	}
}, false);

document.body.addEventListener("touchend", function (e) {
	if (e.target == canvas) {
		e.preventDefault();
	}
}, false);

document.body.addEventListener("touchmove", function (e) {
	if (e.target == canvas) {
		e.preventDefault();
	}
}, false);

// Get the position of the mouse relative to the canvas
function getMousePos(canvasDom, mouseEvent) {
	var rect = canvasDom.getBoundingClientRect();
	return {
		x: mouseEvent.clientX - rect.left,
		y: mouseEvent.clientY - rect.top
	};
}

// Get the position of a touch relative to the canvas
function getTouchPos(canvasDom, touchEvent) {
	var rect = canvasDom.getBoundingClientRect();
	return {
		x: touchEvent.touches[0].clientX - rect.left,
		y: touchEvent.touches[0].clientY - rect.top
	};
}
