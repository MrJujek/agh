import React from 'react';

interface ButtonProps {
    onIncrement: () => void;
}

const Button: React.FC<ButtonProps> = ({ onIncrement }) => {
    return (
        <button
            onClick={onIncrement}
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
            onMouseOver={(e) => e.currentTarget.style.backgroundColor = '#535bf2'}
            onMouseOut={(e) => e.currentTarget.style.backgroundColor = '#646cff'}
        >
            Add
        </button>
    );
};

export default Button;
