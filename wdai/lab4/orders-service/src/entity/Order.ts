import { Entity, PrimaryGeneratedColumn, Column } from "typeorm"

@Entity()
export class Order {
    @PrimaryGeneratedColumn()
    id!: number

    @Column()
    userId!: number

    @Column()
    bookId!: number

    @Column()
    quantity!: number
}
