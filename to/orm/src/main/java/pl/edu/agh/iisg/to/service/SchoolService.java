package pl.edu.agh.iisg.to.service;

import pl.edu.agh.iisg.to.dao.CourseDao;
import pl.edu.agh.iisg.to.dao.GradeDao;
import pl.edu.agh.iisg.to.dao.StudentDao;
import pl.edu.agh.iisg.to.model.Course;
import pl.edu.agh.iisg.to.model.Grade;
import pl.edu.agh.iisg.to.model.Student;
import pl.edu.agh.iisg.to.session.TransactionService;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import static java.util.Collections.emptyMap;

public class SchoolService {

    private final TransactionService transactionService;

    private final StudentDao studentDao;

    private final CourseDao courseDao;

    private final GradeDao gradeDao;

    public SchoolService(TransactionService transactionService, StudentDao studentDao, CourseDao courseDao, GradeDao gradeDao) {
        this.transactionService = transactionService;
        this.studentDao = studentDao;
        this.courseDao = courseDao;
        this.gradeDao = gradeDao;
    }

    public boolean enrollStudent(final Course course, final Student student) {
        if (course.studentSet().contains(student) || student.courseSet().contains(course)) {
            return false;
        }

        return transactionService.doAsTransaction(() -> {
            course.studentSet().add(student);
            student.courseSet().add(course);
            return true;
        }).orElse(false);
    }

    public boolean removeStudent(int indexNumber) {
        return transactionService.doAsTransaction(() -> {
            Optional<Student> studentOptional = studentDao.findByIndexNumber(indexNumber);
            if (studentOptional.isEmpty()) {
                return false;
            }
            Student student = studentOptional.get();
            for (Course course : new HashSet<>(student.courseSet())) {
                course.studentSet().remove(student);
                student.courseSet().remove(course);
            }
            return studentDao.remove(student);
        }).orElse(false);
    }

    public boolean gradeStudent(final Student student, final Course course, final float gradeValue) {
        return transactionService.doAsTransaction(() -> {
            Grade grade = new Grade(student, course, gradeValue);
            gradeDao.save(grade);
            student.gradeSet().add(grade);
            course.gradeSet().add(grade);
            return true;
        }).orElse(false);
    }

    public Map<String, List<Float>> getStudentGrades(String courseName) {
        return transactionService.doAsTransaction(() -> {
            Optional<Course> courseOptional = courseDao.findByName(courseName);
            if (courseOptional.isEmpty()) {
                return Collections.<String, List<Float>>emptyMap();
            }

            Course course = courseOptional.get();
            Map<String, List<Float>> report = new HashMap<>();

            for (Student student : course.studentSet()) {
                List<Float> grades = student.gradeSet().stream()
                        .filter(grade -> grade.course().id() == course.id())
                        .map(Grade::grade)
                        .sorted()
                        .toList();
                report.put(student.fullName(), grades);
            }

            return report;
        }).orElse(emptyMap());
    }
}
