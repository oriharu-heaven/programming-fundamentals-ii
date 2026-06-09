const DIFFICULTIES = {
  easy:   { cols: 9,  rows: 9,  mines: 10 },
  medium: { cols: 16, rows: 16, mines: 40 },
  hard:   { cols: 30, rows: 16, mines: 99 },
};

let state = {};

function getConfig() {
  const key = document.getElementById('difficulty').value;
  return DIFFICULTIES[key];
}

function initGame() {
  const { cols, rows, mines } = getConfig();

  state = {
    cols,
    rows,
    mines,
    cells: [],       // { mine, revealed, flagged, count }
    minesLeft: mines,
    gameOver: false,
    won: false,
    started: false,
    timerInterval: null,
    seconds: 0,
  };

  for (let i = 0; i < rows * cols; i++) {
    state.cells.push({ mine: false, revealed: false, flagged: false, count: 0 });
  }

  clearInterval(state.timerInterval);
  document.getElementById('time').textContent = '0';
  document.getElementById('mines-left').textContent = mines;
  document.getElementById('reset-btn').textContent = '🙂';
  document.getElementById('message').className = 'hidden';

  renderBoard();
}

function placeMines(safeIndex) {
  const { cols, rows, mines } = state;
  const total = cols * rows;
  const indices = [...Array(total).keys()].filter(i => i !== safeIndex);

  for (let i = indices.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [indices[i], indices[j]] = [indices[j], indices[i]];
  }

  for (let i = 0; i < mines; i++) {
    state.cells[indices[i]].mine = true;
  }

  for (let idx = 0; idx < total; idx++) {
    if (state.cells[idx].mine) continue;
    state.cells[idx].count = neighbors(idx).filter(n => state.cells[n].mine).length;
  }
}

function index(row, col) {
  return row * state.cols + col;
}

function neighbors(idx) {
  const row = Math.floor(idx / state.cols);
  const col = idx % state.cols;
  const result = [];
  for (let dr = -1; dr <= 1; dr++) {
    for (let dc = -1; dc <= 1; dc++) {
      if (dr === 0 && dc === 0) continue;
      const r = row + dr, c = col + dc;
      if (r >= 0 && r < state.rows && c >= 0 && c < state.cols) {
        result.push(index(r, c));
      }
    }
  }
  return result;
}

function reveal(idx) {
  const cell = state.cells[idx];
  if (cell.revealed || cell.flagged) return;

  cell.revealed = true;

  if (cell.mine) {
    state.gameOver = true;
    cell.mineHit = true;
    endGame(false);
    return;
  }

  if (cell.count === 0) {
    neighbors(idx).forEach(n => reveal(n));
  }
}

function startTimer() {
  state.seconds = 0;
  state.timerInterval = setInterval(() => {
    state.seconds++;
    document.getElementById('time').textContent = state.seconds;
  }, 1000);
}

function endGame(won) {
  clearInterval(state.timerInterval);
  state.gameOver = true;
  state.won = won;

  if (!won) {
    document.getElementById('reset-btn').textContent = '😵';
    // Reveal all mines
    state.cells.forEach(cell => {
      if (cell.mine) cell.revealed = true;
    });
  } else {
    document.getElementById('reset-btn').textContent = '😎';
  }

  renderBoard();

  const msg = document.getElementById('message');
  msg.textContent = won ? '🎉 クリア！' : '💥 ゲームオーバー';
  msg.className = won ? 'win' : 'lose';
}

function checkWin() {
  const { cells, mines } = state;
  const unrevealed = cells.filter(c => !c.revealed).length;
  if (unrevealed === mines) {
    endGame(true);
  }
}

function handleLeftClick(idx) {
  if (state.gameOver) return;
  const cell = state.cells[idx];
  if (cell.flagged || cell.revealed) return;

  if (!state.started) {
    state.started = true;
    placeMines(idx);
    startTimer();
  }

  reveal(idx);
  if (!state.gameOver) checkWin();
  renderBoard();
}

function handleRightClick(idx, e) {
  e.preventDefault();
  if (state.gameOver) return;
  const cell = state.cells[idx];
  if (cell.revealed) return;

  cell.flagged = !cell.flagged;
  state.minesLeft += cell.flagged ? -1 : 1;
  document.getElementById('mines-left').textContent = state.minesLeft;
  renderBoard();
}

function handleChordClick(idx) {
  const cell = state.cells[idx];
  if (!cell.revealed || cell.count === 0) return;

  const ns = neighbors(idx);
  const flaggedCount = ns.filter(n => state.cells[n].flagged).length;
  if (flaggedCount !== cell.count) return;

  ns.forEach(n => {
    if (!state.cells[n].flagged) reveal(n);
  });
  if (!state.gameOver) checkWin();
  renderBoard();
}

function cellContent(cell) {
  if (!cell.revealed) return cell.flagged ? '🚩' : '';
  if (cell.mine) return '💣';
  return cell.count > 0 ? cell.count : '';
}

function renderBoard() {
  const board = document.getElementById('board');
  board.style.gridTemplateColumns = `repeat(${state.cols}, 32px)`;

  const total = state.rows * state.cols;

  if (board.children.length !== total) {
    board.innerHTML = '';
    for (let i = 0; i < total; i++) {
      const div = document.createElement('div');
      div.className = 'cell';
      div.addEventListener('click', () => handleLeftClick(i));
      div.addEventListener('contextmenu', (e) => handleRightClick(i, e));
      div.addEventListener('dblclick', () => handleChordClick(i));
      board.appendChild(div);
    }
  }

  for (let i = 0; i < total; i++) {
    const cell = state.cells[i];
    const div = board.children[i];
    const classes = ['cell'];
    if (cell.revealed) classes.push('revealed');
    if (cell.flagged) classes.push('flagged');
    if (cell.mineHit) classes.push('mine-hit');
    div.className = classes.join(' ');
    div.textContent = cellContent(cell);
    div.dataset.count = cell.revealed && !cell.mine ? cell.count : '';
  }
}

document.getElementById('new-game').addEventListener('click', initGame);
document.getElementById('reset-btn').addEventListener('click', initGame);
document.getElementById('difficulty').addEventListener('change', initGame);

initGame();
