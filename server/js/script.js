
function mousePressed() {
  return 0;
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
  box.down = 0;

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

let frame = 0;

let joystick_center_x;
let joystick_center_y;
let joystick_radius;

let joy_drawn = 0;

function draw_buttons()
{
  A.draw();
  B.draw();
}

function joy_touch(touch_x, touch_y) {
  if (joy_drawn == 1)
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
  joy_drawn = 1;
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
  joy_drawn = 0;
}

function check_touches()
{
  A.down = 0;
  B.down = 0;
  var joy = 0;
  fill('red');
  
  for (var i = 0; i < touches.length; i++) {
    if (touches[i].x < width / 3 * 2)
    {
      joy_touch(touches[i].x,touches[i].y);
      joy = 1;
    }
    else
    {
      if (Box_vs_point(A,touches[i]))
        A.down = 1;
  
      if (Box_vs_point(B,touches[i]))
        B.down = 1;
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
    body: "Your mom is thick, not gonna lie!",
    headers: {"Content-Type": data}
  })
}





function setup() {
  createCanvas(windowWidth, windowHeight);

  joystick_center_y = (height / 2);
  joystick_radius = height / 6 * 2;
  joystick_center_x = height / 5 * 2;

  A =  new Box(windowWidth - 150,30,100,100);
  B =  new Box(windowWidth - 200, 145,100,100);
  STICK = vec2(0,0);
  // frameRate(30);
}



function draw() {

  // frame++;
  // if (frame % 10 == 0)
  {
    var Data_String = "";
    send_HTTP_request("POST","/",Data_String.concat(
      A.down.toString(), " ",
      B.down.toString(), " ",
      STICK.x.toString(), " ",
      STICK.y.toString()));
  }

  background(255);

  draw_buttons();

  draw_joy_field();

  check_touches();
}

