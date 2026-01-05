import React, { useState } from 'react';

const Login: React.FC = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [repeatPassword, setRepeatPassword] = useState('');

    const handleLogin = () => {
        if (password !== repeatPassword) {
            alert("Passwords do not match");
        } else {
            alert("Logged in successfully");
        }
    };

    const isButtonDisabled = !username || !password || !repeatPassword;

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
            <h3 style={{ color: '#fff', margin: 0, textAlign: 'center' }}>Login</h3>

            <div style={{ display: 'flex', flexDirection: 'column', gap: '0.5rem' }}>
                <label style={{ fontSize: '0.9rem', color: '#aaa' }}>Username</label>
                <input
                    type="text"
                    value={username}
                    onChange={(e) => setUsername(e.target.value)}
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
                <label style={{ fontSize: '0.9rem', color: '#aaa' }}>Password</label>
                <input
                    type="password"
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
                    type="password"
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

            <button
                onClick={handleLogin}
                disabled={isButtonDisabled}
                style={{
                    marginTop: '1rem',
                    backgroundColor: isButtonDisabled ? '#444' : '#646cff',
                    color: isButtonDisabled ? '#888' : 'white',
                    border: 'none',
                    padding: '0.8rem',
                    borderRadius: '8px',
                    fontSize: '1rem',
                    fontWeight: 600,
                    cursor: isButtonDisabled ? 'not-allowed' : 'pointer',
                    transition: 'all 0.2s ease'
                }}
            >
                Log In
            </button>
        </div>
    );
};

export default Login;
