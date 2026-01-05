import React from 'react';

interface Student {
    name: string;
    surname: string;
    year: number;
}

const Students: React.FC = () => {
    const students: Student[] = [
        { name: "John", surname: "Doe", year: 2023 },
        { name: "Jane", surname: "Smith", year: 2022 },
        { name: "Alice", surname: "Johnson", year: 2024 }
    ];

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '600px',
            margin: '1rem auto'
        }}>
            <h3 style={{ color: '#fff', margin: '0 0 1rem 0', textAlign: 'center' }}>Students List</h3>

            <table style={{
                width: '100%',
                borderCollapse: 'collapse',
                color: '#e0e0e0',
                fontSize: '0.95rem'
            }}>
                <thead>
                    <tr style={{ borderBottom: '1px solid #444' }}>
                        <th style={{ padding: '0.8rem', textAlign: 'left' }}>Name</th>
                        <th style={{ padding: '0.8rem', textAlign: 'left' }}>Surname</th>
                        <th style={{ padding: '0.8rem', textAlign: 'left' }}>Year</th>
                    </tr>
                </thead>
                <tbody>
                    {students.map((student, index) => (
                        <tr key={index} style={{ borderBottom: '1px solid #333' }}>
                            <td style={{ padding: '0.8rem' }}>{student.name}</td>
                            <td style={{ padding: '0.8rem' }}>{student.surname}</td>
                            <td style={{ padding: '0.8rem' }}>{student.year}</td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
};

export default Students;
