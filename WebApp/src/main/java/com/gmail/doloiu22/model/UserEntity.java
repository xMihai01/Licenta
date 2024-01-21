package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

@Getter @Setter
@Entity
@Table(name="users")
public class UserEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Basic
    @Column(name = "license_plate")
    private String licensePlate;

    @Basic
    @Column(name = "password")
    private String password;

    @Transient
    private String repeatPassword;

}
