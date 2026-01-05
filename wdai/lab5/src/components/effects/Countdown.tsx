import React, { useState, useEffect } from 'react';

const Countdown: React.FC = () => {
    const [count, setCount] = useState(15.0);
    const [isRunning, setIsRunning] = useState(false);
    const [isFinished, setIsFinished] = useState(false);

    useEffect(() => {
        let interval: ReturnType<typeof setInterval>;

        if (isRunning && count > 0) {
            interval = setInterval(() => {
                setCount(prev => {
                    const next = Math.max(0, prev - 0.1);
                    if (next <= 0.01) {
                        setIsFinished(true);
                        setIsRunning(false);
                        return 0;
                    }
                    return next;
                });
            }, 100);
        }

        return () => {
            if (interval) clearInterval(interval);
        };
    }, [isRunning, count]);

    const handleToggle = () => {
        if (!isFinished) {
            setIsRunning(!isRunning);
        }
    };

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '300px',
            margin: '1rem auto',
            textAlign: 'center'
        }}>
            <h3 style={{ color: '#fff', margin: '0 0 1rem 0' }}>Countdown</h3>

            <div style={{
                fontSize: '3rem',
                fontWeight: 'bold',
                marginBottom: '1rem',
                color: isFinished ? '#ff4646' : '#646cff',
                fontFamily: 'monospace'
            }}>
                {count.toFixed(1)}s
            </div>

            <button
                onClick={handleToggle}
                disabled={isFinished}
                style={{
                    backgroundColor: isFinished ? '#444' : (isRunning ? '#ff4646' : '#646cff'),
                    color: isFinished ? '#888' : 'white',
                    border: 'none',
                    padding: '0.6rem 1.2rem',
                    borderRadius: '8px',
                    fontSize: '1rem',
                    cursor: isFinished ? 'not-allowed' : 'pointer',
                    transition: 'all 0.2s',
                    width: '100%'
                }}
            >
                {isFinished ? "Countdown finished" : (isRunning ? "STOP" : "START")}
            </button>
        </div>
    );
};

export default Countdown;
