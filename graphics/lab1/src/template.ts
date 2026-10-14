import "./style.css";

import * as THREE from "three";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls.js";

window.addEventListener("load", function () {
  const scene = new THREE.Scene();

  const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    10000,
  );
  camera.position.set(100, 100, 100);

  const renderer = new THREE.WebGLRenderer();
  renderer.setClearColor(0x444);
  renderer.setSize(window.innerWidth, window.innerHeight);

  document.getElementById("app")!.appendChild(renderer.domElement);

  const axes = new THREE.AxesHelper(1000);
  scene.add(axes);
  camera.lookAt(scene.position);

  const geometry = new THREE.BoxGeometry(20, 20, 20, 3, 3, 3);
  const material = new THREE.MeshBasicMaterial({
    color: 0x8888ff,
    side: THREE.DoubleSide,
    wireframe: true,
    transparent: true,
    opacity: 0.8,
  });
  const cube = new THREE.Mesh(geometry, material);
  scene.add(cube);

  const controls = new OrbitControls(camera, renderer.domElement);

  function render() {
    requestAnimationFrame(render);
    console.log("render leci");

    controls.update();
    renderer.render(scene, camera);
  }

  render();

  window.addEventListener("resize", onWindowResize, false);

  function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize(window.innerWidth, window.innerHeight);
  }
});
