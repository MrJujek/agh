import React, { useState } from 'react';

const Form: React.FC = () => {
    const [text, setText] = useState('');

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '400px',
            margin: '1rem auto',
            display: 'flex',
            flexDirection: 'column',
            gap: '1rem'
        }}>
            <h3 style={{ color: '#fff', margin: 0, textAlign: 'center' }}>Live Input</h3>
            <input
                type="text"
                value={text}
                onChange={(e) => setText(e.target.value)}
                placeholder="Type something..."
                style={{
                    padding: '0.8rem',
                    borderRadius: '8px',
                    border: '1px solid #444',
                    backgroundColor: '#1a1a1a',
                    color: 'white',
                    fontSize: '1rem',
                    outline: 'none'
                }}
            />
            <div style={{
                padding: '1rem',
                backgroundColor: 'rgba(255, 255, 255, 0.02)',
                borderRadius: '8px',
                minHeight: '2rem',
                color: '#e0e0e0',
                wordBreak: 'break-all'
            }}>
                {text}
            </div>
        </div>
    );
};

export default Form;
