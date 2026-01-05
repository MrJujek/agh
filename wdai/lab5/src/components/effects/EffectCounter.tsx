import React, { useState, useEffect } from 'react';

const EffectCounter: React.FC = () => {
    const [count, setCount] = useState(0);

    useEffect(() => {
        console.log("Hello world");
    }, []);

    useEffect(() => {
        console.log(`Counter increased to ${count}`);
    }, [count]);

    const increment = () => {
        setCount(prev => prev + 1);
    };

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            textAlign: 'center',
            maxWidth: '300px',
            margin: '1rem auto'
        }}>
            <h3 style={{ color: '#fff', marginBottom: '1rem' }}>Effect Counter</h3>
            <div style={{
                fontSize: '3rem',
                fontWeight: 'bold',
                marginBottom: '1rem',
                color: '#646cff'
            }}>
                {count}
            </div>
            <button
                onClick={increment}
                style={{
                    backgroundColor: '#646cff',
                    color: 'white',
                    border: 'none',
                    padding: '0.6rem 1.2rem',
                    borderRadius: '8px',
                    fontSize: '1rem',
                    cursor: 'pointer',
                    transition: 'background-color 0.2s'
                }}
            >
                Add
            </button>
        </div>
    );
};

export default EffectCounter;
