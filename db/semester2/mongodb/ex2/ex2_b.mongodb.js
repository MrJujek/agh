db.dropDatabase();

// ============== VARIANT B: Pure References (Relational Approach) ==============

// Lecturers Collection
db.createCollection("lecturers", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "email" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            email: { bsonType: "string" },
            department: { bsonType: "string" }
         }
      }
   }
});

// Students Collection (no courses_ids array)
db.createCollection("students", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "student_id" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            student_id: { bsonType: "string" },
            email: { bsonType: "string" }
         }
      }
   }
});

// Courses Collection (no enrolled_students array)
db.createCollection("courses", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "name", "lecturer_id" ],
         properties: {
            name: { bsonType: "string" },
            lecturer_id: { bsonType: "objectId" },
            semester: { bsonType: "int" },
            credits: { bsonType: "int" }
         }
      }
   }
});

// Enrollments Collection (join table for student-course relationship)
db.createCollection("enrollments", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "enrollment_date" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            enrollment_date: { bsonType: "date" }
         }
      }
   }
});

// Grades Collection
db.createCollection("grades", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "grade", "date" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            grade: { bsonType: "double" },
            date: { bsonType: "date" }
         }
      }
   }
});

// Course Ratings Collection
db.createCollection("course_ratings", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "rating" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            rating: { bsonType: "int" },
            comment: { bsonType: "string" },
            date: { bsonType: "date" }
         }
      }
   }
});

// ============== Sample Data ==============
const lecturerId = new ObjectId();
const studentId1 = new ObjectId();
const studentId2 = new ObjectId();
const courseId = new ObjectId();
const courseId2 = new ObjectId();

// Insert lecturer
db.lecturers.insertOne({
   _id: lecturerId,
   first_name: "John",
   last_name: "Smith",
   email: "john.smith@university.edu",
   department: "Mathematics"
});

// Insert students
db.students.insertMany([
   {
      _id: studentId1,
      first_name: "Alice",
      last_name: "Johnson",
      student_id: "S12345",
      email: "alice.johnson@student.edu"
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu"
   }
]);

// Insert courses
db.courses.insertMany([
   {
      _id: courseId,
      name: "Discrete Mathematics",
      lecturer_id: lecturerId,
      semester: 1,
      credits: 5
   },
   {
      _id: courseId2,
      name: "Linear Algebra",
      lecturer_id: lecturerId,
      semester: 2,
      credits: 5
   }
]);

// Insert enrollments
db.enrollments.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      enrollment_date: new Date("2025-09-01")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      enrollment_date: new Date("2025-09-01")
   }
]);

// Insert grades
db.grades.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      grade: 4.5,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      grade: 3.8,
      date: new Date("2025-12-15")
   }
]);

// Insert course ratings
db.course_ratings.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      rating: 5,
      comment: "Excellent course!",
      date: new Date("2025-12-20")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      rating: 4,
      comment: "Good content",
      date: new Date("2025-12-20")
   }
]);

// ============== QUERIES ==============

// ========== ADVANTAGES OF VARIANT B ==========

// Query 1: Enroll student to course (advantage: single insert, no consistency issues)
console.log("\n========== ADVANTAGE 1: Enroll Student to Course (Single Insert) ==========");
const enrollResult = db.enrollments.insertOne({
    student_id: studentId1,
    course_id: courseId2,
    enrollment_date: new Date()
});
console.log("Single Insert - No synchronization needed:");
console.log(JSON.stringify(enrollResult, null, 2));

// Query 2: Unenroll student from course (advantage: single delete, no consistency issues)
console.log("\n========== ADVANTAGE 2: Unenroll Student from Course (Single Delete) ==========");
const unenrollResult = db.enrollments.deleteOne({
    student_id: studentId1,
    course_id: courseId2
});
console.log("Single Delete - No synchronization needed:");
console.log(JSON.stringify(unenrollResult, null, 2));

// Query 3: No redundancy - data in one place
console.log("\n========== ADVANTAGE 3: No Data Redundancy (Pure References) ==========");
console.log("All relations stored in separate, normalized collections:");
console.log("- Students are just student info");
console.log("- Courses are just course info");
console.log("- Enrollments keep the relationship");
console.log("- Grades are separate");
console.log("- Course ratings are separate");

// ========== DISADVANTAGES OF VARIANT B ==========

// Query 4: Get student profile with enrolled courses (many lookups)
console.log("\n========== DISADVANTAGE 1: Student Profile with Enrolled Courses (3 Lookups) ==========");
const query1Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "student_id",
            as: "my_enrollments"
        }
    },
    { $unwind: "$my_enrollments" },
    {
        $lookup: {
            from: "courses",
            localField: "my_enrollments.course_id",
            foreignField: "_id",
            as: "course_details"
        }
    },
    { $unwind: "$course_details" },
    {
        $lookup: {
            from: "lecturers",
            localField: "course_details.lecturer_id",
            foreignField: "_id",
            as: "lecturer_details"
        }
    },
    { $unwind: "$lecturer_details" }
]).toArray();

console.log(JSON.stringify(query1Result, null, 2));

// Query 5: Get student grades for all courses (many lookups)
console.log("\n========== DISADVANTAGE 2: Student Grades for All Courses (Multiple Lookups) ==========");
const query5Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "grades",
            localField: "_id",
            foreignField: "student_id",
            as: "grades"
        }
    },
    {
        $lookup: {
            from: "courses",
            localField: "grades.course_id",
            foreignField: "_id",
            as: "course_details"
        }
    }
]).toArray();

console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Get course with all enrolled students and their grades (many lookups)
console.log("\n========== DISADVANTAGE 3: Course with Enrolled Students and Grades (4 Lookups) ==========");
const query6Result = db.courses.aggregate([
    { $match: { _id: courseId } },
    {
        $lookup: {
            from: "lecturers",
            localField: "lecturer_id",
            foreignField: "_id",
            as: "lecturer"
        }
    },
    { $unwind: "$lecturer" },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "course_id",
            as: "enrollments"
        }
    },
    { $unwind: "$enrollments" },
    {
        $lookup: {
            from: "students",
            localField: "enrollments.student_id",
            foreignField: "_id",
            as: "student_info"
        }
    },
    { $unwind: "$student_info" },
    {
        $lookup: {
            from: "grades",
            let: { student: "$student_info._id", course: "$_id" },
            pipeline: [
                { $match: { $expr: { $and: [{ $eq: ["$student_id", "$$student"] }, { $eq: ["$course_id", "$$course"] }] } } }
            ],
            as: "student_grades"
        }
    }
]).toArray();

console.log(JSON.stringify(query6Result, null, 2));

// Query 7: Get course ratings for a course (multiple lookups)
console.log("\n========== DISADVANTAGE 4: Course Ratings (Multiple Lookups) ==========");
const query7Result = db.course_ratings.aggregate([
    { $match: { course_id: courseId } },
    {
        $lookup: {
            from: "students",
            localField: "student_id",
            foreignField: "_id",
            as: "student_info"
        }
    },
    { $unwind: "$student_info" },
    {
        $lookup: {
            from: "courses",
            localField: "course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" }
]).toArray();

console.log(JSON.stringify(query7Result, null, 2));

// Query 8: Get attendance list for a course (many lookups)
console.log("\n========== DISADVANTAGE 5: Course Attendance List (Multiple Lookups) ==========");
const query8Result = db.courses.aggregate([
    { $match: { _id: courseId } },
    {
        $lookup: {
            from: "lecturers",
            localField: "lecturer_id",
            foreignField: "_id",
            as: "lecturer_info"
        }
    },
    { $unwind: "$lecturer_info" },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "course_id",
            as: "enrollments_data"
        }
    },
    {
        $lookup: {
            from: "students",
            let: { student_id: "$enrollments_data.student_id" },
            pipeline: [
                { $match: { $expr: { $in: ["$_id", "$$student_id"] } } }
            ],
            as: "student_list"
        }
    }
]).toArray();

console.log(JSON.stringify(query8Result, null, 2));

// Query 9: Average grade per course (many lookups)
console.log("\n========== DISADVANTAGE 6: Course Average Grade (Aggregation with Lookups) ==========");
const query9Result = db.grades.aggregate([
    { $match: { course_id: courseId } },
    {
        $group: {
            _id: "$course_id",
            average_grade: { $avg: "$grade" },
            total_grades: { $sum: 1 }
        }
    },
    {
        $lookup: {
            from: "courses",
            localField: "_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            course_name: "$course_info.name",
            average_grade: 1,
            total_grades: 1
        }
    }
]).toArray();
console.log(JSON.stringify(query9Result, null, 2));