db.dropDatabase();

// ============== VARIANT A: Balanced Hybrid Approach ==============

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

// Students Collection (contains array of course_ids - controlled redundancy)
db.createCollection("students", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "student_id" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            student_id: { bsonType: "string" },
            email: { bsonType: "string" },
            course_ids: {
               bsonType: "array",
               items: { bsonType: "objectId" },
               description: "Array of ObjectIds - enrolled courses"
            }
         }
      }
   }
});

// Courses Collection (contains enrolled_students array - controlled redundancy)
db.createCollection("courses", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "name", "lecturer_id", "enrolled_students" ],
         properties: {
            name: { bsonType: "string" },
            lecturer_id: { bsonType: "objectId" },
            semester: { bsonType: "int" },
            credits: { bsonType: "int" },
            enrolled_students: {
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "student_id", "student_number" ],
                  properties: {
                     student_id: { bsonType: "objectId" },
                     student_number: { bsonType: "string" }
                  }
               }
            }
         }
      }
   }
});

// Grades Collection (separate - dynamic data)
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

// Course Ratings Collection (separate - dynamic data)
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
const courseId1 = new ObjectId();
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
      email: "alice.johnson@student.edu",
      course_ids: [ courseId1, courseId2 ]
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu",
      course_ids: [ courseId1 ]
   }
]);

// Insert courses
db.courses.insertMany([
   {
      _id: courseId1,
      name: "Discrete Mathematics",
      lecturer_id: lecturerId,
      semester: 1,
      credits: 5,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" },
         { student_id: studentId2, student_number: "S12346" }
      ]
   },
   {
      _id: courseId2,
      name: "Data Structures",
      lecturer_id: lecturerId,
      semester: 2,
      credits: 4,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" }
      ]
   }
]);

// Insert grades
db.grades.insertMany([
   {
      student_id: studentId1,
      course_id: courseId1,
      grade: 4.5,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId2,
      course_id: courseId1,
      grade: 3.8,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId1,
      course_id: courseId2,
      grade: 4.2,
      date: new Date("2025-12-16")
   }
]);

// Insert course ratings
db.course_ratings.insertMany([
   {
      student_id: studentId1,
      course_id: courseId1,
      rating: 5,
      comment: "Excellent course!",
      date: new Date("2025-12-20")
   },
   {
      student_id: studentId2,
      course_id: courseId1,
      rating: 4,
      comment: "Good content",
      date: new Date("2025-12-20")
   }
]);

// ============== QUERIES ==============

// ========== ADVANTAGES OF VARIANT A ==========

// Query 1: Get course attendance list (advantage: single document read)
console.log("\n========== ADVANTAGE 1: Course Attendance List (Single Document) ==========");
const query1Result = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(query1Result, null, 2));

// Query 2: Get student enrolled courses (single lookup - efficient)
console.log("\n========== ADVANTAGE 2: Student with Enrolled Courses (Only 1 Lookup) ==========");
const query2Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "courses",
            localField: "course_ids",
            foreignField: "_id",
            as: "course_details"
        }
    },
    {
        $lookup: {
            from: "lecturers",
            localField: "course_details.lecturer_id",
            foreignField: "_id",
            as: "lecturer_info"
        }
    }
]).toArray();
console.log(JSON.stringify(query2Result, null, 2));

// Query 3: Get course ratings (simple queries with references)
console.log("\n========== ADVANTAGE 3: Course Ratings (Simple Structure) ==========");
const query3Result = db.course_ratings.aggregate([
    { $match: { course_id: courseId1 } },
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
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            course: "$course_info.name",
            student: { $concat: ["$student_info.first_name", " ", "$student_info.last_name"] },
            rating: 1,
            comment: 1
        }
    }
]).toArray();
console.log(JSON.stringify(query3Result, null, 2));

// ========== DISADVANTAGES OF VARIANT A ==========

// Query 4: Get student grades with course details (requires joins)
console.log("\n========== DISADVANTAGE 1: Student Grades (Requires Joins) ==========");
const query4Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "grades",
            localField: "_id",
            foreignField: "student_id",
            as: "grades"
        }
    },
    { $unwind: "$grades" },
    {
        $lookup: {
            from: "courses",
            localField: "grades.course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            student_name: { $concat: ["$first_name", " ", "$last_name"] },
            course_name: "$course_info.name",
            grade: "$grades.grade"
        }
    }
]).toArray();
console.log(JSON.stringify(query4Result, null, 2));

// Query 5: Get course with all students and their grades (complex aggregation)
console.log("\n========== DISADVANTAGE 2: Course with Students and Grades (Complex) ==========");
const query5Result = db.courses.aggregate([
    { $match: { _id: courseId1 } },
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
            from: "grades",
            localField: "_id",
            foreignField: "course_id",
            as: "grades_data"
        }
    },
    {
        $lookup: {
            from: "students",
            let: { student_id: "$enrolled_students.student_id" },
            pipeline: [
                { $match: { $expr: { $in: ["$_id", "$$student_id"] } } }
            ],
            as: "student_details"
        }
    }
]).toArray();
console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Enroll student to course (DISADVANTAGE: requires 2 updates - maintain consistency)
console.log("\n========== DISADVANTAGE 3: Enroll Student to Course (2 Updates Required) ==========");
const enrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $push: { course_ids: courseId2 } }
);
console.log("Update 1 - Add course to student:");
console.log(JSON.stringify(enrollResult1, null, 2));

const enrollResult2 = db.courses.updateOne(
    { _id: courseId2 },
    { $push: { enrolled_students: { student_id: studentId2, student_number: "S12346" } } }
);
console.log("Update 2 - Add student to course (MUST keep both in sync):");
console.log(JSON.stringify(enrollResult2, null, 2));

// Query 7: Unenroll student from course (DISADVANTAGE: requires 2 updates - maintain consistency)
console.log("\n========== DISADVANTAGE 4: Unenroll Student from Course (2 Updates Required) ==========");
const unenrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $pull: { course_ids: courseId1 } }
);
console.log("Update 1 - Remove course from student:");
console.log(JSON.stringify(unenrollResult1, null, 2));

const unenrollResult2 = db.courses.updateOne(
    { _id: courseId1 },
    { $pull: { enrolled_students: { student_id: studentId2 } } }
);
console.log("Update 2 - Remove student from course (MUST keep both in sync):");
console.log(JSON.stringify(unenrollResult2, null, 2));