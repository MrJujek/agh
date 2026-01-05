import React from 'react';
import Produkt from './Product';

const Cart: React.FC = () => {
    return (
        <div style={{
            padding: '2rem',
            backgroundColor: '#242424',
            borderRadius: '16px',
            border: '1px solid #333',
            maxWidth: '800px',
            margin: '0 auto',
            boxShadow: '0 20px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04)'
        }}>
            <h2 style={{
                color: '#fff',
                marginBottom: '1.5rem',
                textAlign: 'center',
                fontSize: '2rem',
                background: 'linear-gradient(to right, #646cff, #9089fc)',
                WebkitBackgroundClip: 'text',
                WebkitTextFillColor: 'transparent'
            }}>
                Your cart
            </h2>
            <div style={{
                display: 'flex',
                flexWrap: 'wrap',
                gap: '1.5rem',
                justifyContent: 'center'
            }}>
                <Produkt nazwa="Apple" />
                <Produkt nazwa="Pear" />
                <Produkt nazwa="Banana" />
                <Produkt nazwa="Orange" />
                <Produkt nazwa="Grapes" />
            </div>
        </div>
    );
};

export default Cart;
