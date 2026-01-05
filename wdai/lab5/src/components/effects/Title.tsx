import React, { useState, useEffect } from 'react';

const Title: React.FC = () => {
    const [title, setTitle] = useState('');

    useEffect(() => {
        if (title) {
            document.title = title;
        }
    }, [title]);

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '300px',
            margin: '1rem auto',
            display: 'flex',
            flexDirection: 'column',
            gap: '1rem'
        }}>
            <h3 style={{ color: '#fff', margin: 0, textAlign: 'center' }}>Page Title</h3>
            <input
                type="text"
                value={title}
                onChange={(e) => setTitle(e.target.value)}
                placeholder="Change page title..."
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
        </div>
    );
};

export default Title;
