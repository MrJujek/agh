import { useEffect, useState } from "react";
import "../style/App.css";
import Square from "../components/Square";
import Circle from "../components/Circle";
import Rectangle from "../components/Rectangle";
import { Link } from "react-router-dom";

export interface IElement {
  id: number;
  element: string;
  data: string;
}

function Main() {
  const getID = () => {
    let id = 0;
    if (elements && elements.length && elements.length > 0) {
      id = elements[elements.length - 1].id + 1;
    } else if (elements && elements.length && elements.length == 0) {
      id = elements[0].id + 1;
    }
    return id;
  };
  const getElement = (element: string) => {
    if (element == "square") {
      return <Square />;
    } else if (element == "circle") {
      return <Circle />;
    } else if (element == "rectangle") {
      return <Rectangle />;
    }
  };

  const [elements, setElements] = useState<IElement[]>([
    { id: 1, element: "circle", data: "" },
    { id: 2, element: "square", data: "" },
    { id: 3, element: "rectangle", data: "" },
  ]);

  const [selected, setSelected] = useState<
    "all" | "square" | "rectangle" | "circle"
  >("all");

  useEffect(() => {
    localStorage.setItem("elements", JSON.stringify(elements));
  }, [elements]);

  const deleteElement = (id: number) => {
    setElements((prev) => prev && prev.filter((element) => element.id != id));
  };
  const [animate, setAnimate] = useState(false);
  useEffect(() => {
    setTimeout(() => {
      setAnimate(true);
      setTimeout(() => {
        setAnimate(false);
      }, 1000);
    }, 2000);
  }, []);
  return (
    <>
      <select
        name="selected"
        id=""
        defaultValue={selected}
        onChange={(e) =>
          setSelected(
            e.target.value as "all" | "square" | "rectangle" | "circle"
          )
        }
      >
        <option value={"all"}>All</option>
        <option value={"square"}>Squares</option>
        <option value={"rectangle"}>Rectangles</option>
        <option value={"circle"}>Circles</option>
      </select>
      <button
        onClick={() => {
          setElements((prev) => [
            ...prev,
            { id: getID(), element: "square", data: "asd" },
          ]);
        }}
      >
        Add square
      </button>
      <button
        onClick={() => {
          setElements((prev) => [
            ...prev,
            { id: getID(), element: "rectangle", data: "asd" },
          ]);
        }}
      >
        Add rectangle
      </button>
      <button
        onClick={() => {
          setElements((prev) => [
            ...prev,
            { id: getID(), element: "circle", data: "asd" },
          ]);
        }}
      >
        Add circle
      </button>

      <div className="elements">
        {elements
          .filter((e) => selected == "all" || selected == e.element)
          .map((element) => {
            return (
              <div key={element.id}>
                <div
                  style={
                    animate
                      ? { transform: "scale(0.5)", transition: "1s" }
                      : { transform: "scale(1)", transition: "1s" }
                  }
                >
                  <Link to={`/shape/${element.id}`}>
                    {getElement(element.element)}
                  </Link>
                </div>
                <button
                  onClick={() => {
                    deleteElement(element.id);
                  }}
                >
                  Delete
                </button>
              </div>
            );
          })}
      </div>
    </>
  );
}

export default Main;
