import React, { useState } from 'react';
import Button from './Button';

const NewCounter: React.FC = () => {
    const [count, setCount] = useState(0);

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
            <h3 style={{ color: '#fff', marginBottom: '1rem' }}>New Counter</h3>
            <div style={{
                fontSize: '3rem',
                fontWeight: 'bold',
                marginBottom: '1rem',
                color: '#9089fc'
            }}>
                {count}
            </div>
            <Button onIncrement={increment} />
        </div>
    );
};

export default NewCounter;
