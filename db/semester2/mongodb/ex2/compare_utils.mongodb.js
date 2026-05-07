function canonicalize(v) {
    if (Array.isArray(v)) {
        const mapped = v.map(canonicalize);
        if (mapped.length > 0 && mapped.every(el => el && typeof el === "object" && !Array.isArray(el))) {
            return mapped.sort((a, b) => {
                const sa = JSON.stringify(a);
                const sb = JSON.stringify(b);
                if (sa < sb) return -1;
                if (sa > sb) return 1;
                return 0;
            });
        }
        return mapped;
    }

    if (v && typeof v === "object") {
        const keys = Object.keys(v).sort();
        const res = {};
        for (const k of keys) res[k] = canonicalize(v[k]);
        return res;
    }

    return v;
}

function sortById(arr) {
    return arr.sort((a, b) => {
        if (a._id < b._id) return -1;
        if (a._id > b._id) return 1;
        return 0;
    });
}

function deepEqualWithTolerance(a, b, eps) {
    if (typeof a !== typeof b) return false;

    if (typeof a === "number" && typeof b === "number") {
        return Math.abs(a - b) <= eps;
    }

    if (Array.isArray(a) && Array.isArray(b)) {
        if (a.length !== b.length) return false;
        for (let i = 0; i < a.length; i++) {
            if (!deepEqualWithTolerance(a[i], b[i], eps)) return false;
        }
        return true;
    }

    if (a && typeof a === "object" && b && typeof b === "object") {
        const ka = Object.keys(a);
        const kb = Object.keys(b);
        if (ka.length !== kb.length) return false;
        for (const k of ka) {
            if (!Object.prototype.hasOwnProperty.call(b, k)) return false;
            if (!deepEqualWithTolerance(a[k], b[k], eps)) return false;
        }
        return true;
    }

    return a === b;
}

function toComparableArray(cursor) {
    return sortById(cursor.toArray().map(canonicalize));
}
