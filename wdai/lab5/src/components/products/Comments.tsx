import React, { useEffect, useState } from 'react';
import Comment, { type CommentProps } from './Comment';

interface CommentsResponse {
    comments: CommentProps[];
    total: number;
    skip: number;
    limit: number;
}

const Comments: React.FC = () => {
    const [comments, setComments] = useState<CommentProps[]>([]);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        fetch('https://dummyjson.com/comments')
            .then(res => res.json())
            .then((data: CommentsResponse) => {
                setComments(data.comments);
                setLoading(false);
            })
            .catch(err => {
                console.error("Error fetching comments:", err);
                setLoading(false);
            });
    }, []);

    if (loading) {
        return <div style={{ color: '#fff', textAlign: 'center', padding: '2rem' }}>Loading comments...</div>;
    }

    return (
        <div style={{
            padding: '2rem',
            width: '100%',
            maxWidth: '800px',
            margin: '0 auto'
        }}>
            <h2 style={{
                color: '#fff',
                marginBottom: '2rem',
                textAlign: 'center',
                fontSize: '2rem',
                borderBottom: '2px solid #333',
                paddingBottom: '1rem'
            }}>
                Comments Section
            </h2>
            <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', gap: '1rem' }}>
                {comments.map(comment => (
                    <Comment key={comment.id} {...comment} />
                ))}
            </div>
        </div>
    );
};

export default Comments;
