let start = document.getElementById("start");
let stop = document.getElementById("stop");
let reset = document.getElementById("reset");
let show = document.getElementById("time");

let startDate = null;
let count;
start.addEventListener("click", () => {
  stop.disabled = false;
  reset.disabled = false;
  if (startDate == null) {
    startDate = 0;
  }
  clearInterval(count);
  count = setInterval(() => {
    startDate += 1;
    show.innerText = getCurrent(startDate);
  }, 1000);
});

function getCurrent(seconds) {
  let curr = seconds;
  let h = Math.floor(curr / 3600);
  let m = Math.floor(curr / 60);
  let s = curr % 60;
  let time = "";
  if (h != 0) {
    time += h + "h ";
  }
  if (m != 0) {
    time += m + "m ";
  }
  if (s != 0) {
    time += s + "s";
  }
  return time;
}

reset.addEventListener("click", () => {
  startDate = 0;
  show.innerText = "0s";
  clearInterval(count);
  count = setInterval(() => {
    startDate += 1;
    show.innerText = getCurrent(startDate);
  }, 1000);
});

stop.addEventListener("click", () => {
  clearInterval(count);
});
