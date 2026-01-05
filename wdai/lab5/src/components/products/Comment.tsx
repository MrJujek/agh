import React, { useState } from 'react';

export interface User {
    id: number;
    username: string;
    fullName: string;
}

export interface CommentProps {
    id: number;
    body: string;
    postId: number;
    likes: number;
    user: User;
}

const Comment: React.FC<CommentProps> = ({ id, body, postId, likes: initialLikes, user }) => {
    const [likes, setLikes] = useState(initialLikes);

    const handleLike = () => setLikes(prev => prev + 1);
    const handleDislike = () => setLikes(prev => prev - 1);

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: '#1e1e1e',
            borderRadius: '12px',
            border: '1px solid #333',
            marginBottom: '1rem',
            boxShadow: '0 4px 6px -1px rgba(0, 0, 0, 0.1)',
            maxWidth: '600px',
            width: '100%'
        }}>
            <div style={{
                display: 'flex',
                alignItems: 'center',
                marginBottom: '1rem',
                borderBottom: '1px solid #333',
                paddingBottom: '0.8rem'
            }}>
                <div style={{
                    width: '40px',
                    height: '40px',
                    borderRadius: '50%',
                    backgroundColor: '#646cff',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    marginRight: '1rem',
                    fontWeight: 'bold',
                    color: 'white',
                    fontSize: '1.2rem'
                }}>
                    {user.fullName.charAt(0)}
                </div>
                <div>
                    <div style={{ fontWeight: 600, color: '#fff' }}>{user.fullName}</div>
                    <div style={{ fontSize: '0.8rem', color: '#888' }}>@{user.username}</div>
                </div>
                <div style={{ marginLeft: 'auto', fontSize: '0.8rem', color: '#666' }}>
                    Post #{postId}
                </div>
            </div>

            <div style={{
                color: '#e0e0e0',
                lineHeight: '1.6',
                marginBottom: '1.2rem',
                textAlign: 'left'
            }}>
                {body}
            </div>

            <div style={{
                display: 'flex',
                alignItems: 'center',
                gap: '1rem',
                color: '#aaa',
                fontSize: '0.9rem'
            }}>
                <button
                    onClick={handleLike}
                    style={{
                        background: 'none',
                        border: '1px solid #444',
                        color: '#aaa',
                        cursor: 'pointer',
                        padding: '0.4rem 0.8rem',
                        borderRadius: '6px',
                        display: 'flex',
                        alignItems: 'center',
                        gap: '0.5rem',
                        transition: 'all 0.2s'
                    }}
                    onMouseOver={(e) => {
                        e.currentTarget.style.borderColor = '#646cff';
                        e.currentTarget.style.color = '#646cff';
                    }}
                    onMouseOut={(e) => {
                        e.currentTarget.style.borderColor = '#444';
                        e.currentTarget.style.color = '#aaa';
                    }}
                >
                    ▲ Like
                </button>

                <span style={{ fontWeight: 'bold', color: likes >= 0 ? '#fff' : '#ff4646' }}>
                    {likes}
                </span>

                <button
                    onClick={handleDislike}
                    style={{
                        background: 'none',
                        border: '1px solid #444',
                        color: '#aaa',
                        cursor: 'pointer',
                        padding: '0.4rem 0.8rem',
                        borderRadius: '6px',
                        display: 'flex',
                        alignItems: 'center',
                        gap: '0.5rem',
                        transition: 'all 0.2s'
                    }}
                    onMouseOver={(e) => {
                        e.currentTarget.style.borderColor = '#ff4646';
                        e.currentTarget.style.color = '#ff4646';
                    }}
                    onMouseOut={(e) => {
                        e.currentTarget.style.borderColor = '#444';
                        e.currentTarget.style.color = '#aaa';
                    }}
                >
                    ▼ Dislike
                </button>

                <span style={{ marginLeft: 'auto', fontSize: '0.8rem', color: '#555' }}>
                    Click ID: {id}
                </span>
            </div>
        </div>
    );
};

export default Comment;
