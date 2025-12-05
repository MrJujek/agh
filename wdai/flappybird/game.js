const canvas = document.getElementById("game-canvas");
const ctx = canvas.getContext("2d");

let frames = 0;
const DEGREE = Math.PI / 180;

const assets = {
  bg: new Image(),
  fg: new Image(),
  bird: [new Image(), new Image(), new Image()],
  pipeNorth: new Image(),
  pipeSouth: new Image(),
  numbers: [],
  boosts: {
    ghost: new Image(),
    swiftness: new Image(),
    enderPearl: new Image(),
  },
  enemy: new Image(),
};

assets.bg.src = "assets/Images/background-day.png";
assets.fg.src = "assets/Images/base.png";
assets.bird[0].src = "assets/Images/yellowbird-upflap.png";
assets.bird[1].src = "assets/Images/yellowbird-midflap.png";
assets.bird[2].src = "assets/Images/yellowbird-downflap.png";
assets.pipeSouth.src = "assets/Images/pipe-green.png";
assets.boosts.ghost.src = "assets/Images/boosts/invisibility.gif";
assets.boosts.swiftness.src = "assets/Images/boosts/swiftness.gif";
assets.boosts.enderPearl.src = "assets/Images/boosts/enderpearl.png";
assets.enemy.src = "assets/Images/boosts/red-ghost.png";

for (let i = 0; i < 10; i++) {
  assets.numbers[i] = new Image();
  assets.numbers[i].src = `assets/UI/Numbers/${i}.png`;
}

const sounds = {
  score: new Audio("assets/Sound/point.wav"),
  flap: new Audio("assets/Sound/wing.wav"),
  hit: new Audio("assets/Sound/hit.wav"),
  die: new Audio("assets/Sound/die.wav"),
  swoosh: new Audio("assets/Sound/swoosh.wav"),
  bgm: new Audio("assets/Sound/The-Flappy-Bird-Song.mp3"),
};

sounds.bgm.loop = true;
sounds.bgm.volume = 0.3;

sounds.bgm
  .play()
  .catch((e) => console.log("Autoplay blocked, waiting for interaction"));

const state = {
  current: 0,
  getReady: 0,
  game: 1,
  over: 2,
  newRecordAnim: 3,
};

const boosts = {
  ghost: {
    active: false,
    timer: 0,
    duration: 300,
  },
  swiftness: {
    active: false,
    timer: 0,
    duration: 120,
    speedMultiplier: 3,
  },
};

const startBtn = {
  x: 120,
  y: 263,
  w: 83,
  h: 29,
};

document.addEventListener("keydown", function (evt) {
  if (evt.code === "Space") {
    if (sounds.bgm.paused) {
      sounds.bgm.play();
    }

    switch (state.current) {
      case state.getReady:
        state.current = state.game;
        document.getElementById("start-screen").classList.add("hidden");
        sounds.swoosh.play();
        break;
      case state.game:
        bird.flap();
        break;
      case state.over:
        resetGame();
        break;
      case state.newRecordAnim:
        break;
    }
  }
});

const restartBtn = document.getElementById("restart-btn");
restartBtn.addEventListener("click", function () {
  resetGame();
});

const bg = {
  draw: function () {
    ctx.drawImage(assets.bg, 0, 0, canvas.width, canvas.height);
  },
};

const fg = {
  h: 112,
  x: 0,
  dx: 2,
  draw: function () {
    ctx.drawImage(
      assets.fg,
      this.x,
      canvas.height - this.h,
      canvas.width,
      this.h
    );
    ctx.drawImage(
      assets.fg,
      this.x + canvas.width,
      canvas.height - this.h,
      canvas.width,
      this.h
    );
  },
  update: function () {
    if (state.current === state.game) {
      let speed = this.dx;
      if (boosts.swiftness.active) speed *= boosts.swiftness.speedMultiplier;

      this.x = (this.x - speed) % (canvas.width / 2);
      if (this.x <= -canvas.width) {
        this.x = 0;
      }
    }
  },
};

const bird = {
  animation: [0, 1, 2, 1],
  x: 50,
  y: 150,
  w: 34,
  h: 24,
  radius: 12,
  frame: 0,
  gravity: 0.25,
  jump: 4.6,
  speed: 0,
  rotation: 0,
  spinAngle: 0,

  draw: function () {
    let birdIndex = this.animation[this.frame];
    let birdImg = assets.bird[birdIndex];

    ctx.save();
    ctx.translate(this.x, this.y);

    if (state.current === state.newRecordAnim) {
      ctx.rotate(this.spinAngle);
    } else {
      ctx.rotate(this.rotation);
    }

    if (boosts.ghost.active) {
      ctx.globalAlpha = 0.5;
    }

    ctx.drawImage(birdImg, -this.w / 2, -this.h / 2, this.w, this.h);

    ctx.globalAlpha = 1.0;
    ctx.restore();
  },

  flap: function () {
    this.speed = -this.jump;
    this.rotation = -25 * DEGREE;
    sounds.flap.play();
  },

  update: function () {
    const period = state.current === state.getReady ? 10 : 5;
    this.frame += frames % period === 0 ? 1 : 0;
    this.frame = this.frame % this.animation.length;

    if (state.current === state.getReady) {
      this.y = 150;
      this.rotation = 0 * DEGREE;
    } else if (state.current === state.newRecordAnim) {
      this.y = 150 + Math.sin(frames * 0.1) * 20;
      this.spinAngle += 10 * DEGREE;
    } else {
      this.speed += this.gravity;
      this.y += this.speed;

      if (this.y + this.h / 2 >= canvas.height - fg.h) {
        this.y = canvas.height - fg.h - this.h / 2;
        if (state.current === state.game) {
          if (!boosts.ghost.active) {
            checkGameOver();
          } else {
            this.speed = 0;
            this.flap();
            checkGameOver();
          }
        } else if (state.current === state.over) {
          this.speed = 0;
          if (
            document
              .getElementById("game-over-screen")
              .classList.contains("hidden")
          ) {
            sounds.die.play();
            showGameOver();
          }
        }
      }

      if (this.speed < 0) {
        this.rotation = -25 * DEGREE;
      } else {
        this.rotation += 5 * DEGREE;
        this.frame = 1;
        if (this.rotation > 90 * DEGREE) {
          this.rotation = 90 * DEGREE;
        }
      }
    }
  },
  reset: function () {
    this.speed = 0;
    this.rotation = 0;
    this.y = 150;
    this.frame = 0;
    this.spinAngle = 0;
  },
};

const pipes = {
  position: [],
  w: 52,
  h: 320,
  gap: 100,
  dx: 2,

  draw: function () {
    for (let i = 0; i < this.position.length; i++) {
      let p = this.position[i];
      let topY = p.y;
      let bottomY = p.y + this.h + this.gap;

      ctx.save();
      if (boosts.ghost.active) ctx.globalAlpha = 0.3;

      ctx.save();
      ctx.translate(p.x, topY);
      ctx.scale(1, -1);
      ctx.drawImage(assets.pipeSouth, 0, -this.h, this.w, this.h);
      ctx.restore();

      ctx.drawImage(assets.pipeSouth, p.x, bottomY, this.w, this.h);

      ctx.restore();
    }
  },

  update: function () {
    if (state.current !== state.game) return;

    let speed = this.dx;
    if (boosts.swiftness.active) speed *= boosts.swiftness.speedMultiplier;

    let spawnRate = 80;
    if (boosts.swiftness.active) spawnRate = 33;

    if (frames % spawnRate === 0) {
      if (this.spawnCount >= 2 && Math.random() < 0.2) {
        collectibles.spawn();
      } else {
        this.position.push({
          x: canvas.width,
          y: -150 * (Math.random() + 1),
          passed: false,
        });
        this.spawnCount++;
      }
    }

    for (let i = 0; i < this.position.length; i++) {
      let p = this.position[i];
      p.x -= speed;

      if (!boosts.ghost.active) {
        if (bird.x + bird.radius > p.x && bird.x - bird.radius < p.x + this.w) {
          if (bird.y - bird.radius < p.y + this.h) {
            checkGameOver();
          }
          if (bird.y + bird.radius > p.y + this.h + this.gap) {
            checkGameOver();
          }
        }
      }

      if (p.x + this.w < bird.x - bird.radius && !p.passed) {
        score.value += 1;
        score.best = Math.max(score.value, score.best);
        sounds.score.play();
        p.passed = true;
      }

      if (p.x + this.w <= 0) {
        this.position.shift();
        i--;
      }
    }
  },

  reset: function () {
    this.position = [];
    this.spawnCount = 0;
  },
};

const collectibles = {
  items: [],
  dx: 2,

  spawn: function () {
    let r = Math.random();
    let type = "ghost";
    if (r > 0.66) type = "swiftness";
    else if (r > 0.33) type = "enderPearl";

    this.items.push({
      x: canvas.width,
      y: Math.random() * (canvas.height - fg.h - 100) + 50,
      type: type,
    });
  },

  draw: function () {
    for (let i = 0; i < this.items.length; i++) {
      let item = this.items[i];
      let img;
      if (item.type === "ghost") img = assets.boosts.ghost;
      else if (item.type === "swiftness") img = assets.boosts.swiftness;
      else img = assets.boosts.enderPearl;

      let size = 30;
      ctx.drawImage(img, item.x - size / 2, item.y - size / 2, size, size);
    }
  },

  update: function () {
    if (state.current !== state.game) return;

    let speed = this.dx;
    if (boosts.swiftness.active) speed *= boosts.swiftness.speedMultiplier;

    for (let i = 0; i < this.items.length; i++) {
      let item = this.items[i];
      item.x -= speed;

      let dx = bird.x - item.x;
      let dy = bird.y - item.y;
      let dist = Math.sqrt(dx * dx + dy * dy);

      if (dist < bird.radius + 15) {
        if (item.type === "ghost") {
          boosts.ghost.active = true;
          boosts.ghost.timer = boosts.ghost.duration;
        } else if (item.type === "swiftness") {
          boosts.swiftness.active = true;
          boosts.swiftness.timer = boosts.swiftness.duration;
          boosts.ghost.active = true;
          boosts.ghost.timer = boosts.swiftness.duration;
        } else if (item.type === "enderPearl") {
          pipes.position.forEach((p) => {
            p.x -= 200;
            if (p.x + pipes.w < bird.x - bird.radius && !p.passed) {
              score.value += 1;
              score.best = Math.max(score.value, score.best);
              sounds.score.play();
              p.passed = true;
            }
          });
          collectibles.items.forEach((it) => (it.x -= 200));
          enemies.list.forEach((e) => (e.x -= 200));
          fg.x -= 200;

          boosts.ghost.active = true;
          boosts.ghost.timer = 60;
        }
        sounds.score.play();
        this.items.splice(i, 1);
        i--;
        continue;
      }

      if (item.x < -30) {
        this.items.splice(i, 1);
        i--;
      }
    }
  },

  reset: function () {
    this.items = [];
  },
};

const enemies = {
  list: [],
  dx: 3,

  draw: function () {
    for (let i = 0; i < this.list.length; i++) {
      let e = this.list[i];
      let size = 30;
      ctx.drawImage(assets.enemy, e.x - size / 2, e.y - size / 2, size, size);
    }
  },

  update: function () {
    if (state.current !== state.game) return;

    let speed = this.dx;
    if (boosts.swiftness.active) speed *= boosts.swiftness.speedMultiplier;

    if (frames % 250 === 0 && Math.random() > 0.3) {
      if (!boosts.ghost.active) {
        this.list.push({
          x: canvas.width,
          y: Math.random() * (canvas.height - fg.h - 50) + 50,
        });
      }
    }

    for (let i = 0; i < this.list.length; i++) {
      let e = this.list[i];
      e.x -= speed;

      if (!boosts.ghost.active) {
        let dx = bird.x - e.x;
        let dy = bird.y - e.y;
        let dist = Math.sqrt(dx * dx + dy * dy);

        if (dist < bird.radius + 10) {
          checkGameOver();
        }
      }

      if (e.x < -20) {
        this.list.splice(i, 1);
        i--;
      }
    }
  },

  reset: function () {
    this.list = [];
  },
};

const score = {
  best: 0,
  value: 0,

  draw: function () {
    if (
      state.current === state.game ||
      state.current === state.over ||
      state.current === state.newRecordAnim
    ) {
      let scoreStr = this.value.toString();
      let totalWidth = 0;

      for (let i = 0; i < scoreStr.length; i++) {
        totalWidth += assets.numbers[parseInt(scoreStr[i])].width;
      }

      let x = (canvas.width - totalWidth) / 2;
      let y = 40;

      for (let i = 0; i < scoreStr.length; i++) {
        let num = parseInt(scoreStr[i]);
        ctx.drawImage(assets.numbers[num], x, y);
        x += assets.numbers[num].width;
      }
    }

    const scoreDisplay = document.getElementById("score-display");
    if (scoreDisplay) scoreDisplay.classList.add("hidden");
  },

  reset: function () {
    this.value = 0;
  },
};

function draw() {
  ctx.fillStyle = "#70c5ce";
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  bg.draw();
  pipes.draw();
  collectibles.draw();
  enemies.draw();
  fg.draw();
  bird.draw();
  score.draw();
}

function update() {
  bird.update();
  fg.update();
  pipes.update();
  collectibles.update();
  enemies.update();

  if (state.current === state.game) {
    if (boosts.ghost.active) {
      boosts.ghost.timer--;
      if (boosts.ghost.timer <= 0) boosts.ghost.active = false;
    }
    if (boosts.swiftness.active) {
      boosts.swiftness.timer--;
      if (boosts.swiftness.timer <= 0) boosts.swiftness.active = false;
    }
  }
}

function loop() {
  update();
  draw();
  frames++;
  requestAnimationFrame(loop);
}

function checkGameOver() {
  state.current = state.over;
  sounds.hit.play();

  const scores = getHighScores();
  const currentBest = scores.length > 0 ? scores[0] : 0;

  if (score.value > currentBest) {
    state.current = state.newRecordAnim;
    setTimeout(() => {
      showCongrats();
    }, 3000);
  } else {
    setTimeout(() => sounds.die.play(), 300);
    showGameOver();
  }
}

function showCongrats() {
  document.getElementById("congrats-screen").classList.remove("hidden");
  setTimeout(() => {
    document.getElementById("congrats-screen").classList.add("hidden");
    sounds.die.play();
    showGameOver();
  }, 2000);
}

function showGameOver() {
  const gameOverScreen = document.getElementById("game-over-screen");
  const currentScoreEl = document.getElementById("current-score");
  const bestScoreEl = document.getElementById("best-score");
  const highScoresList = document.getElementById("high-scores-list");

  currentScoreEl.innerHTML = "";
  score.value
    .toString()
    .split("")
    .forEach((char) => {
      const img = document.createElement("img");
      img.src = `assets/UI/Numbers/${char}.png`;
      img.style.height = "20px";
      img.style.width = "auto";
      currentScoreEl.appendChild(img);
    });

  saveScore(score.value);

  const scores = getHighScores();
  score.best = scores.length > 0 ? scores[0] : 0;

  bestScoreEl.innerHTML = "";
  score.best
    .toString()
    .split("")
    .forEach((char) => {
      const img = document.createElement("img");
      img.src = `assets/UI/Numbers/${char}.png`;
      img.style.height = "20px";
      img.style.width = "auto";
      bestScoreEl.appendChild(img);
    });

  highScoresList.innerHTML = "";
  scores.forEach((s, index) => {
    const li = document.createElement("li");
    li.innerHTML = `<span>${index + 1}.</span> <span>${s}</span>`;
    highScoresList.appendChild(li);
  });

  gameOverScreen.classList.remove("hidden");
  document.getElementById("start-screen").classList.add("hidden");
}

function getHighScores() {
  const scores = localStorage.getItem("flappy_highscores");
  return scores ? JSON.parse(scores) : [];
}

function saveScore(newScore) {
  let scores = getHighScores();
  scores.push(newScore);
  scores.sort((a, b) => b - a);
  scores = scores.slice(0, 5);
  localStorage.setItem("flappy_highscores", JSON.stringify(scores));
}

function resetGame() {
  bird.reset();
  pipes.reset();
  collectibles.reset();
  enemies.reset();
  score.reset();

  boosts.ghost.active = false;
  boosts.swiftness.active = false;

  state.current = state.getReady;
  frames = 0;

  document.getElementById("game-over-screen").classList.add("hidden");
  document.getElementById("start-screen").classList.remove("hidden");
  document.getElementById("score-display").classList.remove("hidden");
}

loop();
