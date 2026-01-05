import React, { useState } from 'react';

interface Student {
    name: string;
    surname: string;
    year: number;
}

interface AddStudentProps {
    onAdd: (student: Student) => void;
}

const AddStudent: React.FC<AddStudentProps> = ({ onAdd }) => {
    const [name, setName] = useState('');
    const [surname, setSurname] = useState('');
    const [year, setYear] = useState('');

    const handleSubmit = (e: React.FormEvent) => {
        e.preventDefault();

        if (!name || !surname || !year) {
            alert("All fields are required");
            return;
        }

        const yearNum = parseInt(year);
        if (isNaN(yearNum)) {
            alert("Year must be a number");
            return;
        }

        onAdd({ name, surname, year: yearNum });
        setName('');
        setSurname('');
        setYear('');
    };

    return (
        <form onSubmit={handleSubmit} style={{
            marginTop: '2rem',
            display: 'flex',
            flexDirection: 'column',
            gap: '1rem',
            padding: '1rem',
            backgroundColor: 'rgba(0, 0, 0, 0.2)',
            borderRadius: '8px'
        }}>
            <h4 style={{ margin: 0, color: '#fff' }}>Add Student</h4>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr 1fr', gap: '1rem' }}>
                <input
                    type="text"
                    placeholder="Name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                    style={{
                        padding: '0.6rem',
                        borderRadius: '6px',
                        border: '1px solid #444',
                        backgroundColor: '#1a1a1a',
                        color: 'white'
                    }}
                />
                <input
                    type="text"
                    placeholder="Surname"
                    value={surname}
                    onChange={(e) => setSurname(e.target.value)}
                    style={{
                        padding: '0.6rem',
                        borderRadius: '6px',
                        border: '1px solid #444',
                        backgroundColor: '#1a1a1a',
                        color: 'white'
                    }}
                />
                <input
                    type="text"
                    placeholder="Year"
                    value={year}
                    onChange={(e) => setYear(e.target.value)}
                    style={{
                        padding: '0.6rem',
                        borderRadius: '6px',
                        border: '1px solid #444',
                        backgroundColor: '#1a1a1a',
                        color: 'white'
                    }}
                />
            </div>

            <button
                type="submit"
                style={{
                    backgroundColor: '#646cff',
                    color: 'white',
                    border: 'none',
                    padding: '0.6rem',
                    borderRadius: '6px',
                    cursor: 'pointer',
                    fontWeight: 600
                }}
            >
                Add
            </button>
        </form>
    );
};

const StudentManager: React.FC = () => {
    const [students, setStudents] = useState<Student[]>([
        { name: "John", surname: "Doe", year: 2023 },
        { name: "Jane", surname: "Smith", year: 2022 },
        { name: "Alice", surname: "Johnson", year: 2024 }
    ]);

    const handleAddStudent = (newStudent: Student) => {
        setStudents(prev => [...prev, newStudent]);
    };

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '600px',
            margin: '1rem auto'
        }}>
            <h3 style={{ color: '#fff', margin: '0 0 1rem 0', textAlign: 'center' }}>Student Manager</h3>

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

            <AddStudent onAdd={handleAddStudent} />
        </div>
    );
};

export default StudentManager;
