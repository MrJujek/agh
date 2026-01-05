import React, { useState } from 'react';
import { useNavigate, Link } from 'react-router-dom';
import './Blog.css';

const AddArticle: React.FC = () => {
    const [title, setTitle] = useState('');
    const [content, setContent] = useState('');
    const navigate = useNavigate();

    const handleSubmit = (e: React.FormEvent) => {
        e.preventDefault();

        if (!title.trim() || !content.trim()) return;

        const newArticle = {
            id: Date.now(),
            title,
            content
        };

        const stored = localStorage.getItem('articles');
        const articles = stored ? JSON.parse(stored) : [];
        articles.push(newArticle);

        localStorage.setItem('articles', JSON.stringify(articles));

        navigate('/blog');
    };

    return (
        <div className="blog-container">
            <h2 className="blog-title">Add New Article</h2>
            <form onSubmit={handleSubmit} className="blog-card">
                <div style={{ marginBottom: '1.5rem' }}>
                    <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>Title</label>
                    <input
                        type="text"
                        className="blog-input"
                        value={title}
                        onChange={e => setTitle(e.target.value)}
                        placeholder="Enter article title..."
                        required
                    />
                </div>

                <div style={{ marginBottom: '1.5rem' }}>
                    <label style={{ display: 'block', marginBottom: '0.5rem', fontWeight: 'bold' }}>Content</label>
                    <textarea
                        className="blog-textarea"
                        value={content}
                        onChange={e => setContent(e.target.value)}
                        placeholder="Write your article user..."
                        required
                    />
                </div>

                <div style={{ display: 'flex', gap: '1rem', alignItems: 'center' }}>
                    <button type="submit" className="blog-btn">
                        ADD
                    </button>
                    <Link to="/blog" className="blog-link">Cancel</Link>
                </div>
            </form>
        </div>
    );
};

export default AddArticle;
