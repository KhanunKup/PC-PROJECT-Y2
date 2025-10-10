// ip address arduino
const ARDUINO_IP = "172.20.10.12";

// check state (correct/wrong) in arduino
async function getStatusFromArduino() {
    try {
        const response = await fetch(`http://${ARDUINO_IP}/status`);
        const status = await response.text();
        const resultStatusEl = document.getElementById('result-status');
        const charTextEl = document.getElementById('charText');
        const activeCharBtn = document.querySelector('.char-btn.active');

        const cleanStatus = status.trim(); 

        if (cleanStatus === 'correct' || cleanStatus === 'wrong') {
            resultStatusEl.textContent = cleanStatus.charAt(0).toUpperCase() + cleanStatus.slice(1) + '!';
            resultStatusEl.className = `result-status ${cleanStatus}`; // change color Green/Red

            // update character quote base on state
            if (activeCharBtn) {
                const textKey = cleanStatus === 'correct' ? 'textCorrect' : 'textWrong';
                const newText = activeCharBtn.dataset[textKey];
                charTextEl.textContent = newText;
            }
        }
    } catch (err) {
        // don't show error
    }
}

// send correct answer to arduino
function sendToArduino(binaryValue) {
  fetch(`http://${ARDUINO_IP}/set?ans=${binaryValue}`)
    .then(res => console.log("Sent new answer to Arduino:", binaryValue))
    .catch(err => console.error("Failed to send answer to Arduino:", err));
}

// random value base on mode (easy/hard)
function randInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

// random new quiz
function generateProblem() {
    const ops = ['+', '-'];
    const op = ops[Math.floor(Math.random() * ops.length)];
    const mode = document.querySelector('input[name="mode"]:checked').value;
    const maxVal = mode === 'hard' ? 255 : 15;
    let a, b, result;

    if (op === '+') {
        a = randInt(0, maxVal);
        b = randInt(0, maxVal - a);
        result = a + b;
    } else {
        a = randInt(0, maxVal);
        b = randInt(0, a);
        result = a - b;
    }
    const bin = result.toString(2).padStart(mode === 'hard' ? 8 : 4, '0');
    
    // show quiz on screen
    const problemEl = document.getElementById('problem');
    problemEl.textContent = `${a} ${op} ${b} = ?`;
    problemEl.dataset.answer = String(result);

    // reset UI when random quiz
    document.getElementById('result-status').textContent = '';
    document.getElementById('result-status').className = 'result-status';
    const activeCharBtn = document.querySelector('.char-btn.active');
    if (activeCharBtn) {
        document.getElementById('charText').textContent = activeCharBtn.dataset.textDefault;
    }
    
    // update binary ans
    const showBin = document.getElementById('showBinary').checked;
    const binaryEl = document.getElementById('binary');
    if(showBin) {
        binaryEl.textContent = `Answer (binary ${mode === 'hard' ? '8 bits' : '4 bits'}): ${bin}`;
    } else {
        binaryEl.textContent = '';
    }
    
    sendToArduino(bin);
}

// wait for web loading before start
document.addEventListener('DOMContentLoaded', function() {
    const newBtn = document.getElementById('newBtn');
    const showBin = document.getElementById('showBinary');
    const modeRadios = document.querySelectorAll('input[name="mode"]');

    // event listener
    newBtn.addEventListener('click', generateProblem);
    modeRadios.forEach(radio => radio.addEventListener('change', generateProblem));

    // event listener in real-time
    showBin.addEventListener('change', () => {
        const problemEl = document.getElementById('problem');
        const binaryEl = document.getElementById('binary');
        const mode = document.querySelector('input[name="mode"]:checked').value;

        if (problemEl.dataset.answer !== undefined) {
            if (showBin.checked) {
                const result = Number(problemEl.dataset.answer);
                const bin = result.toString(2).padStart(mode === 'hard' ? 8 : 4, '0');
                binaryEl.textContent = `Answer (binary ${mode === 'hard' ? '8 bits' : '4 bits'}): ${bin}`;
            } else {
                binaryEl.textContent = '';
            }
        }
    });

    // ask for new state for arduino every 2 sec
    setInterval(getStatusFromArduino, 2000);

    // --- character selection logic ---
    const charBtns = document.querySelectorAll('.char-btn');
    const charImg = document.getElementById('charImage');
    const charText = document.getElementById('charText');

    charBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            charBtns.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            charImg.src = btn.dataset.img;
            charText.textContent = btn.dataset.textDefault; // set character default quote when swap
        });
    });
});