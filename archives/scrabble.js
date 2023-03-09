var colors=[
  "#1a1c2c", // 0 black
  "#5d275d", // 1 med black
  "#b13e53", // 2 red
  "#ef7d57", // 3 brown
  "#ffcd75", // 4 orange
  "#a7f070", // 5 yellow
  "#38b764", // 6 green
  "#257179", // 7 dark green
  "#29366f", // 8 dark blue
  "#3b5dc9", // 9 blue
  "#41a6f6", // 10 light blue
  "#73eff7", // 11 light white
  "#f4f4f4", // 12 dim white
  "#94b0c2", // 13 light gray
  "#566c86", // 14 gray
  "#333c57"  // 15 dark gray
];


var letters=[
  'A','B','C','D','E','F','G','H','I',
  'J','K','L','M','N','O','P','Q','R',
  'S','T','U','V','W','X','Y','Z',' '
];

var points=[
  1,3,3,2,1,4,2,4,1,
  8,5,1,3,1,1,3,10,1,
  1,1,1,4,4,8,4,10,0
];

var pieces=[
  9,2,2,4,12,2,3,2,9,
  1,1,4,2,6,8,2,1,6,
  4,6,4,2,2,1,2,1,2
];

var bcolors=[
  colors[3],
  colors[10],
  colors[9],
  colors[5],
  colors[2],
  colors[5],
]

var bonus=[
  4,0,0,1,0,0,0,4,0,0,0,1,0,0,4,
  0,3,0,0,0,2,0,0,0,2,0,0,0,3,0,
  0,0,3,0,0,0,1,0,1,0,0,0,3,0,0,
  1,0,0,3,0,0,0,1,0,0,0,3,0,0,1,
  0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,
  0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,
  0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,
  4,0,0,1,0,0,0,5,0,0,0,1,0,0,4,
  0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,
  0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,
  0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,
  1,0,0,3,0,0,0,1,0,0,0,3,0,0,1,
  0,0,3,0,0,0,1,0,1,0,0,0,3,0,0,
  0,3,0,0,0,2,0,0,0,2,0,0,0,3,0,
  4,0,0,1,0,0,0,4,0,0,0,1,0,0,4
];

var board=new Array(15*15);
for(var i=0;i<15*15;i++) board[i]=' ';


var canvas=document.getElementById("canvas"); 
var ctx=canvas.getContext("2d");

document.body.style.backgroundColor=colors[8];

canvas.width = 34*17+2;
canvas.height = 34*17+2;


ctx.translate(0.5,0.5);

ctx.fillStyle=colors[0];
ctx.fillRect(0,0,canvas.width,canvas.height);


function drawLetter(l,x,y,b) {
  var n=0;
  if(l.charCodeAt(0)===32)
    n=26;
  else
    n=l.toUpperCase().charCodeAt(0)-65;
  
  ctx.fillStyle=colors[12];
  ctx.fillRect(x*34+34+2,y*34+34+2,32,32);
  
  ctx.fillStyle=colors[0];
  ctx.beginPath();
  ctx.rect(x*34+34+2,y*34+34+2,32,32);
  ctx.stroke();
  ctx.closePath();
  
  if(b) ctx.fillStyle=colors[10];
  else ctx.fillStyle=colors[0];
  ctx.font="bold 18px monospace";
  ctx.textBaseline="middle";
  ctx.textAlign="center";
  ctx.fillText(letters[n],x*34+34+2+14,y*34+34+2+16);

  if(!b) {
  ctx.font="10px monospace";
  ctx.textBaseline="middle";
  ctx.textAlign="center";
  ctx.fillText(points[n]==0?"":points[n].toString(),x*34+34+2+24,y*34+34+2+26);
  }
}

function drawBoard(x,y) {
  ctx.font="12px monospace";
  ctx.textBaseline="middle";
  ctx.textAlign="center";
  for(var j=0;j<15;j++) {
    for(var i=0;i<15;i++) {
      var k=j*15+i;
      ctx.fillStyle=bcolors[bonus[k]];
      ctx.fillRect(i*34+2+x,j*34+2+y,32,32);
      ctx.fillStyle=colors[12];
      ctx.fillText(String.fromCharCode(65+i),i*34+34+16+2,16+2);
      ctx.fillText(String.fromCharCode(65+i),i*34+34+16+2,15*34+34+16+2);
      ctx.fillText((j+1).toString(),16+2,j*34+34+16+2);
      ctx.fillText((j+1).toString(),15*34+34+16+2,j*34+34+16+2);
    }
  }
  ctx.font="24px monospace";
  ctx.fillStyle=colors[0];
  ctx.fillText(String.fromCharCode(9733),7*34+34+16+2,7*34+34+16+2);
}

function drawText(t,x0,y0,d) {
  var x=x0.toUpperCase().charCodeAt(0)-65;
  var y=y0-1;
  for(var i=0;i<t.length;i++) {
    var c=t.charAt(i);
    if(c===c.toUpperCase()) {
      b=true;
    } else {
      b=false;
    }
    if(d.toUpperCase()==='A') {
      if(board[y*15+(x+i)]===' ') {
        board[y*15+(x+i)]=c;
        drawLetter(c,i+x,y,b);
      }
    } else if(d.toUpperCase()==='D') { 
      if(board[(y+i)*15+x]===' ') {
        board[(y+i)*15+x]=c;
       drawLetter(c,x,i+y,b);
      }
    }
  }
}

drawBoard(34,34);
