import React from 'react';

const Ternary: React.FC = () => {
    const a = true;
    const b = false;

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '400px',
            margin: '1rem auto'
        }}>
            <h3 style={{ color: '#fff', margin: '0 0 1rem 0', textAlign: 'center' }}>Ternary Operator</h3>

            <div style={{ marginBottom: '0.5rem', color: '#e0e0e0' }}>
                {a ? "Statement a is true" : "Statement a is false"}
            </div>

            <div style={{ color: '#e0e0e0' }}>
                {b ? "Statement b is true" : "Statement b is false"}
            </div>
        </div>
    );
};

export default Ternary;
