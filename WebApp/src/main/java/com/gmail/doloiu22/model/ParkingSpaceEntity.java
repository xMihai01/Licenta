package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

@Getter @Setter
@Entity
@Table(name="parking_space")
public class ParkingSpaceEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Basic
    @Column(name = "camera_id")
    private Long cameraID;

    @Basic
    @Column(name = "x1")
    private Integer x1;

    @Basic
    @Column(name = "x2")
    private Integer x2;

    @Basic
    @Column(name = "y1")
    private Integer y1;

    @Basic
    @Column(name = "y2")
    private Integer y2;

    @Basic
    @Column(name = "name")
    private String name;

}
