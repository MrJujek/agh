import React, { useState, useEffect } from 'react';

const Licznik: React.FC = () => {
    // Lazy initial state to read from localStorage
    const [count, setCount] = useState<number>(() => {
        const saved = localStorage.getItem('licznik_8_1');
        return saved ? parseInt(saved, 10) : 0;
    });

    useEffect(() => {
        localStorage.setItem('licznik_8_1', count.toString());
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
            <h3 style={{ color: '#fff', marginBottom: '1rem' }}>Licznik (LocalStorage)</h3>
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
                Dodaj
            </button>
        </div>
    );
};

export default Licznik;
