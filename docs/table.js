var table ='<table align="left" id="toc"><tbody><tr><th>Table of Contents</th></tr><tr><td><a href="index.html">Home Page</a></td></tr><tr><td><a href="update.html">Changelog</a></td></tr><tr><td><a href="manual.html">Tutorials</a></td></tr><tr><td><a href="video.html">Videos</a></td></tr><tr><td><a href="download.html">Download Links</a></td></tr><tr><td><a href="https://www.patreon.com/GK4EVER">Donations</a></td></tr></tbody></table>';


document.getElementById("body").innerHTML+= table;


window.onscroll = function() {myFunction()};


var toc = document.getElementById("toc");


var sticky = toc.offsetTop;


function myFunction() {
  if (window.pageYOffset >= sticky) {
    toc.classList.add("sticky")
  } else {
    toc.classList.remove("sticky");
  }
} 