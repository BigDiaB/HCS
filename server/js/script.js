
function mousePressed() {
  return false;
}

document.addEventListener('gesturestart', function(e) {
  e.preventDefault();
});

function vec2(x, y)
{
  var vec = {};
  vec.x = x;
  vec.y = y;
  return vec;
}

function AABB(pos1, pos2, size1, size2)
{
  return (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y);
}


function Box_vs_point(box,point)
{
  return AABB(box.pos,point,box.size,vec2(1,1));
}

function Box(x,y,w,h)
{
  var box = {};
  box.pos = {};
  box.size = {};
  box.pos.x = x;
  box.pos.y = y;
  box.size.x = w;
  box.size.y = h;
  box.down = false;
  box.last_down = false;
  box.pressed = false;
  box.released = false;

  box.draw = function()
  {
    if (this.down)
      fill("black");
    else
      fill("grey");
    rect(this.pos.x,this.pos.y,this.size.x,this.size.y);
  }
  return box;
}

let A;
let B;
let SICK;
let x_diff = 0;
let y_diff = 0;

let joystick_center_x;
let joystick_center_y;
let joystick_radius;

let joy_drawn = false;

function draw_buttons()
{
  A.draw();
  B.draw();
}

function update_buttons()
{

  if (A.down && !A.last_down)
    A.pressed = true;
  else
    A.pressed = false;
  if (!A.down && A.last_down)
    A.released = true;
  else
    A.released = false;

  if (B.down && !B.last_down)
    B.pressed = true;
  else
    B.pressed = false;
  if (!B.down && B.last_down)
    B.released = true;
  else
    B.released = false;


  A.last_down = A.down;
  A.down = false;
  // A.pressed = false;
  // A.released = false;
  B.last_down = B.down;
  B.down = false;
  // B.pressed = false;
  // B.released = false;
}

function joy_touch(touch_x, touch_y) {
  if (joy_drawn == true)
    return;
  x_diff = int(touch_x - joystick_center_x);
  y_diff = int(joystick_center_y - touch_y);


  if (x_diff < -100)
    x_diff = -100;
  else if (x_diff > 100)
    x_diff = 100;

  if (y_diff < -100)
    y_diff = -100;
  else if (y_diff > 100)
    y_diff = 100;

  push();
  translate(joystick_center_x, joystick_center_y);
  pop();
  push();
  fill("grey");
  rectMode(CENTER);
  ellipse(touch_x, touch_y, joystick_radius, joystick_radius);
  line(joystick_center_x, joystick_center_y, touch_x, touch_y);
  pop();
  joy_drawn = true;
}

function draw_joy_field()
{
  push();
  translate(joystick_center_x, joystick_center_y);
  rotate(QUARTER_PI);
  line(-joystick_radius, 0, joystick_radius, 0);
  rotate(HALF_PI);
  line(-joystick_radius, 0, joystick_radius, 0);
  noFill();
  circle(0, 0, joystick_radius * 2);
  pop();

  fill(50);
  STICK.x = x_diff;
  STICK.y = y_diff;

  x_diff = 0;
  y_diff = 0;
  joy_drawn = false;
}

function check_touches()
{
  var joy = false;
  fill('red');
  
  for (var i = 0; i < touches.length; i++) {
    if (touches[i].x < width / 3 * 2)
    {
      joy_touch(touches[i].x,touches[i].y);
      joy = true;
    }
    else
    {
      if (Box_vs_point(A,touches[i]))
        A.down = true;
      if (Box_vs_point(B,touches[i]))
        B.down = true;
    }
    ellipse(touches[i].x, touches[i].y, 10, 10);
  }
  if (!joy)
  {
    fill("grey");
    ellipse(joystick_center_x, joystick_center_y, joystick_radius, joystick_radius);
  }
}



function send_HTTP_request(method, url, data) {
  return fetch(url, {
    method: method,
    body: JSON.stringify(data),
    headers: data ? {"Content-Type": "application/json"} : {}
  })
  .then(response => {
    return response.json();
  });
}





function setup() {
  createCanvas(windowWidth, windowHeight);

  joystick_center_y = (height / 2);
  joystick_radius = height / 6 * 2;
  joystick_center_x = height / 5 * 2;

  A =  new Box(windowWidth - 150,30,100,100);
  B =  new Box(windowWidth - 200, 145,100,100);
  STICK = vec2(0,0);
}



function draw() {
  background(255);

  draw_buttons();

  draw_joy_field();

  update_buttons();

  check_touches();

  var AB = {};
  AB.down = A.down; 
  AB.pressed = A.pressed; 
  AB.released = A.released;

  var BB = {};
  BB.down = B.down; 
  BB.pressed = B.pressed; 
  BB.released = B.released;

  var Data = {};
  Data.A = AB;
  Data.B = BB;
  Data.Stick = STICK;

  send_HTTP_request("POST","/",Data)
}

