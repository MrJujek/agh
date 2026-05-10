db.dropDatabase();

// ============== VARIANT C: Hybrid with Nested Approach ==============

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

// Students Collection (contains course_ids array + nested course_ratings)
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
               items: { bsonType: "objectId" }
            },
            course_ratings: { // Nested ratings given by student to courses
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "course_id", "rating" ],
                  properties: {
                     course_id: { bsonType: "objectId" },
                     rating: { bsonType: "int" },
                     comment: { bsonType: "string" },
                     date: { bsonType: "date" }
                  }
               }
            }
         }
      }
   }
});

// Courses Collection (contains enrolled_students array + nested grades)
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
            },
            grades: { // Nested grades assigned to students in this course
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "student_id", "grade", "date" ],
                  properties: {
                     student_id: { bsonType: "objectId" },
                     grade: { bsonType: "double" },
                     date: { bsonType: "date" }
                  }
               }
            }
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
      course_ids: [ courseId1, courseId2 ],
      course_ratings: [
         {
            course_id: courseId1,
            rating: 5,
            comment: "Excellent course!",
            date: new Date("2025-12-20")
         },
         {
            course_id: courseId2,
            rating: 4,
            comment: "Good, but challenging",
            date: new Date("2025-12-21")
         }
      ]
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu",
      course_ids: [ courseId1 ],
      course_ratings: [
         {
            course_id: courseId1,
            rating: 4,
            comment: "Good content",
            date: new Date("2025-12-20")
         }
      ]
   }
]);

// Insert courses with nested grades
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
      ],
      grades: [
         {
            student_id: studentId1,
            grade: 4.5,
            date: new Date("2025-12-15")
         },
         {
            student_id: studentId2,
            grade: 3.8,
            date: new Date("2025-12-15")
         }
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
      ],
      grades: [
         {
            student_id: studentId1,
            grade: 4.2,
            date: new Date("2025-12-16")
         }
      ]
   }
]);

// ============== QUERIES ==============

// ========== ADVANTAGES OF VARIANT C ==========

// Query 1: Get course with all grades in one document (advantage)
console.log("\n========== ADVANTAGE 1: Course Card with All Grades (Single Document) ==========");
const query1Result = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(query1Result, null, 2));

// Query 2: Get student ratings (nested in student - easy access)
console.log("\n========== ADVANTAGE 2: Student Course Ratings (Nested in Student) ==========");
const query2Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    { $unwind: "$course_ratings" },
    {
        $lookup: {
            from: "courses",
            localField: "course_ratings.course_id",
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
            rating: "$course_ratings.rating",
            comment: "$course_ratings.comment",
            date: "$course_ratings.date"
        }
    }
]).toArray();
console.log(JSON.stringify(query2Result, null, 2));

// Query 3: Add a new grade to course (easy - just push)
console.log("\n========== ADVANTAGE 3: Adding a New Grade to Course (Simple Push) ==========");
const addGradeResult = db.courses.updateOne(
    { _id: courseId2 },
    {
        $push: {
            grades: {
                student_id: studentId2,
                grade: 3.9,
                date: new Date("2025-12-16")
            }
        }
    }
);
console.log("Add Grade Result (single operation):");
console.log(JSON.stringify(addGradeResult, null, 2));

// Verify grades were added
console.log("\nVerification - Updated Grades in Course:");
const courseWithNewGrade = db.courses.findOne(
    { _id: courseId2 },
    { name: 1, grades: 1 }
);
console.log(JSON.stringify(courseWithNewGrade, null, 2));

// Query 4: Add a new course rating to student (simple push)
console.log("\n========== ADVANTAGE 4: Adding a New Course Rating to Student (Simple Push) ==========");
const addRatingResult = db.students.updateOne(
    { _id: studentId2 },
    {
        $push: {
            course_ratings: {
                course_id: courseId2,
                rating: 3,
                comment: "Interesting but difficult",
                date: new Date("2025-12-22")
            }
        }
    }
);
console.log("Add Rating Result (single operation):");
console.log(JSON.stringify(addRatingResult, null, 2));

// Verify ratings were added
console.log("\nVerification - Updated Student with New Rating:");
const studentWithNewRating = db.students.findOne(
    { _id: studentId2 },
    { first_name: 1, last_name: 1, course_ratings: 1 }
);
console.log(JSON.stringify(studentWithNewRating, null, 2));

// ========== DISADVANTAGES OF VARIANT C ==========

// Query 5: Get all grades for a specific student (disadvantage: requires unwinding)
console.log("\n========== DISADVANTAGE 1: All Grades for a Student (Requires $unwind All Courses) ==========");
const query5Result = db.courses.aggregate([
    { $unwind: "$grades" },
    { $match: { "grades.student_id": studentId1 } },
    {
        $project: {
            _id: 0,
            course_name: "$name",
            student_grade: "$grades.grade",
            grade_date: "$grades.date"
        }
    }
]).toArray();
console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Update a single grade using array filters (disadvantage: complex)
console.log("\n========== DISADVANTAGE 2: Updating a Grade (Array Filters Required) ==========");
const updateResult = db.courses.updateOne(
    { _id: courseId1, "grades.student_id": studentId1 },
    { $set: { "grades.$[elem].grade": 4.7 } },
    { arrayFilters: [{ "elem.student_id": studentId1 }] }
);
console.log("Update Result (requires arrayFilters):");
console.log(JSON.stringify(updateResult, null, 2));

// Verify the update
console.log("\nVerification - Updated Course:");
const updatedCourse = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(updatedCourse, null, 2));

// Query 7: Course average grade (disadvantage: requires $unwind)
console.log("\n========== DISADVANTAGE 3: Course Average Grade (Requires $unwind) ==========");
const query7Result = db.courses.aggregate([
    { $match: { _id: courseId1 } },
    { $unwind: "$grades" },
    {
        $group: {
            _id: "$_id",
            course_name: { $first: "$name" },
            average_grade: { $avg: "$grades.grade" },
            total_students: { $sum: 1 }
        }
    }
]).toArray();
console.log(JSON.stringify(query7Result, null, 2));

// Query 8: Enroll student to course (disadvantage: requires 2 updates + document grows)
console.log("\n========== DISADVANTAGE 4: Enroll Student to Course (2 Updates + Document Grows) ==========");
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
console.log("Update 2 - Add student to course (document size grows):");
console.log(JSON.stringify(enrollResult2, null, 2));

// Query 9: Unenroll student from course (disadvantage: requires 2 updates)
console.log("\n========== DISADVANTAGE 5: Unenroll Student from Course (2 Updates) ==========");
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
console.log("Update 2 - Remove student from course:");
console.log(JSON.stringify(unenrollResult2, null, 2));