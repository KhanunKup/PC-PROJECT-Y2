const ARDUINO_IP = "172.20.10.12";  // arduini local IP

function sendToArduino(binaryValue) {
  fetch(`http://${ARDUINO_IP}/set?ans=${binaryValue}`)
    .then(res => console.log("Sent to Arduino:", binaryValue))
    .catch(err => console.error("Failed to send:", err));
}

// ans is in range(0,15) (4 bits binary)
function randInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function toBinary(n) {
    return n.toString(2);
}

function generateProblem() {
    // random operator (+,-)
    const ops = ['+', '-'];
    const op = ops[Math.floor(Math.random() * ops.length)];

    // check difficulty
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
    
    // show QUIZ
    const problemEl = document.getElementById('problem');
    problemEl.textContent = `${a} ${op} ${b} = ?`;
    problemEl.dataset.answer = String(result);

    // show ANS if checkbox is TRUE
    const showBinary = document.getElementById('showBinary').checked;
    const binaryEl = document.getElementById('binary');
    if (showBinary) {
        binaryEl.textContent = `Answer (binary ${mode === 'hard' ? '8 bits' : '4 bits'}): ${bin}`;
    } else {
        binaryEl.textContent = ''; // if player don't select check box
    }

    // send binary ans to arduino
    sendToArduino(bin);
}

// set event listener
document.addEventListener('DOMContentLoaded', function() {
    const newBtn = document.getElementById('newBtn');
    const showBin = document.getElementById('showBinary')
    newBtn.addEventListener('click', generateProblem);
    showBin.addEventListener('change', () => {
        //if quiz exist, update binary follow checkbox
        const problemEl = document.getElementById('problem');
        if (problemEl.dataset.answer !== undefined) {
            const result = Number(problemEl.dataset.answer);
            const binaryEl = document.getElementById('binary');
            if (showBin.checked) {
                binaryEl.textContent = 'Answer (binary): ' + toBinary(result);
                binaryEl.setAttribute('aria-hidden', 'false');
            } else {
                binaryEl.textContent = '';
                binaryEl.setAttribute('aria-hidden', 'true');
            }
        }
    });

    // random quiz when load page
    generateProblem();
    // charactor selection
            const charBtns=document.querySelectorAll('.char-btn');
            const charImg=document.getElementById('charImage');
            const charText=document.getElementById('charText');

            charBtns.forEach(btn=>{
                btn.addEventListener('click',()=>{
                    charBtns.forEach(b=>b.classList.remove('active'));
                    btn.classList.add('active');
                    charImg.src=btn.dataset.img;
                    charText.textContent=btn.dataset.text;
                });
            });
});

