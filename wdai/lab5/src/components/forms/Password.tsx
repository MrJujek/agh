import React, { useState } from 'react';

const Password: React.FC = () => {
    const [password, setPassword] = useState('');
    const [repeatPassword, setRepeatPassword] = useState('');

    const getMessage = () => {
        if (!password && !repeatPassword) {
            return "Please enter password";
        }
        if (password !== repeatPassword) {
            return "Passwords do not match";
        }
        return "";
    };

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
            <h3 style={{ color: '#fff', margin: 0, textAlign: 'center' }}>Password Check</h3>

            <div style={{ display: 'flex', flexDirection: 'column', gap: '0.5rem' }}>
                <label style={{ fontSize: '0.9rem', color: '#aaa' }}>Password</label>
                <input
                    type="text"
                    value={password}
                    onChange={(e) => setPassword(e.target.value)}
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

            <div style={{ display: 'flex', flexDirection: 'column', gap: '0.5rem' }}>
                <label style={{ fontSize: '0.9rem', color: '#aaa' }}>Repeat Password</label>
                <input
                    type="text"
                    value={repeatPassword}
                    onChange={(e) => setRepeatPassword(e.target.value)}
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

            <div style={{
                marginTop: '0.5rem',
                minHeight: '1.5rem',
                color: password !== repeatPassword && (password || repeatPassword) ? '#ff4646' : '#aaa',
                textAlign: 'center',
                fontWeight: 500
            }}>
                {getMessage()}
            </div>
        </div>
    );
};

export default Password;
